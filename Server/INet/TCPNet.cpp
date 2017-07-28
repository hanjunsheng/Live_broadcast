#include "TCPNet.h"
#include <process.h>
#include "AbsIMediator.h"

//任务类
class CMyTask :public ITask
{
public:
	CMyTask(CTCPNet * ptcp)
	{
		this->ptcpnet = ptcp;
	}
	~CMyTask(){}
public:
	void RunITask();
private:
	DWORD dwNumberOfBytes;
	SOCKET CompletionKey;
	MYSOCKETEX * pmysocket;
	BOOL result;
	CTCPNet * ptcpnet;
};

void CMyTask::RunITask()
{
	while(1)
	{
		result = GetQueuedCompletionStatus(ptcpnet->m_ComIoPort,&dwNumberOfBytes,(PULONG_PTR)&CompletionKey,(LPOVERLAPPED*)&pmysocket,INFINITE);
		if(result == TRUE)
		{
			if(pmysocket && CompletionKey)
			{
				switch (pmysocket->type)
				{
				case NT_ACCEPT:
					{
						cout<<"客户端连接成功"<<endl;
						//再次投递请求
						ptcpnet->PostAccept();
						CreateIoCompletionPort((HANDLE)pmysocket->socket,ptcpnet->m_ComIoPort,pmysocket->socket,0);
						ptcpnet->PostRecv(pmysocket);

						//投一个任务
						CMyTask * ptask = new CMyTask(ptcpnet);
						ptcpnet->m_threadpool.PushItask(ptask);

						//获得连接的IP
						sockaddr_in *addrremove = NULL;
						sockaddr_in *addrlocal = NULL;
						int size,size1;
						GetAcceptExSockaddrs(pmysocket->szbuf,0,sizeof(sockaddr_in)+16,sizeof(sockaddr_in)+16,(sockaddr **)&addrlocal,&size1,(sockaddr **)&addrremove,&size);

						//处理
						((CABsIMediator *)ptcpnet->m_pme)->DealTCPaccept(addrremove,pmysocket->socket,pmysocket->szbuf);

					}
					break;
				case NT_READ:
					{	//处理
						ptcpnet->m_pme->DealTCPData(pmysocket,pmysocket->szbuf);
						//再次投递请求
						ptcpnet->PostRecv(pmysocket);
					}
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			list<MYSOCKETEX *>::iterator ite = ptcpnet->m_lstSocket.begin();
			while(ite != ptcpnet->m_lstSocket.end())
			{
				if(pmysocket == *ite)
				{
					delete *ite;
					*ite = NULL;
					ite = ptcpnet->m_lstSocket.erase(ite);
					break;
				}
				++ite;
			}
		}
	}
}


//TCP类
CTCPNet::CTCPNet(CIMediator * pme)
{
	m_pme = pme;
	m_socket = NULL;
	m_ComIoPort = NULL;
}

CTCPNet::~CTCPNet()
{

}

bool CTCPNet::Initial()
{
	//1.加载库
	WORD wVersionRequested;
    WSADATA wsaData;

	wVersionRequested = MAKEWORD(2,2);
	WSAStartup(wVersionRequested,&wsaData);

	if(LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		return false;
	}

	//创建socket
	m_socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		UnInitial();
		return false;
	}

	//绑定
	sockaddr_in sockaddrserve;
	sockaddrserve.sin_family = AF_INET;
	sockaddrserve.sin_port = htons(DEAFAULT_TCP_PORT);
	sockaddrserve.sin_addr.s_addr = CINet::GetHostIp();

	int nsize = sizeof(sockaddr_in);
	if(SOCKET_ERROR == bind(m_socket,(const sockaddr *)&sockaddrserve,nsize))
	{
		closesocket(m_socket);
		WSACleanup();
		return false;
	}

	//获得系统的核数
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//监听listen
	if(SOCKET_ERROR == listen(m_socket,si.dwNumberOfProcessors * 2))
	{
		UnInitial();
		return false;
	}

	//创建完成端口
	this->m_ComIoPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,0);
	if(m_ComIoPort == NULL)
	{
		UnInitial();
		return false;
	}

	//将监听socket交给完成端口管理
	CreateIoCompletionPort((HANDLE)m_socket,m_ComIoPort,m_socket,0);

	//投递accept请求
	while(this->PostAccept() == false);
	
	////创建线程池
	m_threadpool.CreateThreadPool(1,si.dwNumberOfProcessors * 2);

	//创建一个任务，等待连接
	CMyTask * ptask = new CMyTask(this);
	this->m_threadpool.PushItask(ptask);
	
	return true;
}

bool CTCPNet::PostAccept()
{
	DWORD dwBytesReceived;
	MYSOCKETEX * pmysocket = new MYSOCKETEX;
	pmysocket->socket = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);
	pmysocket->type = NT_ACCEPT;
	pmysocket->wv.hEvent = WSACreateEvent();

	if(!AcceptEx(m_socket,pmysocket->socket,pmysocket->szbuf,0,sizeof(sockaddr_in)+16,sizeof(sockaddr_in)+16,&dwBytesReceived,&pmysocket->wv))
	{
		if(WSAGetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	
	this->m_lstSocket.push_back(pmysocket);
	
	return true;
}

bool CTCPNet::PostRecv(MYSOCKETEX * pmysocket)
{
	DWORD * pdwNum = NULL;
	list<STRU_REMARCHORINFO *>::iterator ite = ((CABsIMediator *)this->m_pme)->m_lstahchor.begin();
	while(ite != ((CABsIMediator *)this->m_pme)->m_lstahchor.end())
	{
		if(pmysocket->socket == (*ite)->socket)
		{
			pdwNum = &((*ite)->dwNumberOfBytesRecvd);
			break;
		}
		++ite;
	}

	DWORD dwFlags = false;

	pmysocket->type = NT_READ;
	WSABUF wb;
	wb.buf = pmysocket->szbuf;
	wb.len = sizeof(pmysocket->szbuf);

	if(WSARecv(pmysocket->socket,&wb,1,pdwNum,&dwFlags,&pmysocket->wv,NULL))
	{
		if(WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}
	}

	return true;
}

void CTCPNet::UnInitial()
{
	//发送完成端口退出消息
	int nsize = this->m_threadpool.m_runthreadnum;
	while(nsize)
	{
		PostQueuedCompletionStatus(this->m_ComIoPort,NULL,NULL,NULL);
		nsize--;
	}

	Sleep(1000);

	//清空MYSOCKETEX
	list<MYSOCKETEX *>::iterator itesocket = this->m_lstSocket.begin();
	while(itesocket != this->m_lstSocket.end())
	{
		closesocket((*itesocket)->socket);
		(*itesocket)->socket = NULL;
		delete *itesocket;
		*itesocket = NULL;
		++itesocket;
	}

	this->m_threadpool.Destroythreadpool();

	if(m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}
	WSACleanup();
}

bool CTCPNet::Send(sockaddr_in * addr,char * szbuf,int len)
{
	return false;
}