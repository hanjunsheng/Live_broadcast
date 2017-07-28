#include "UDPNet.h"
#include <process.h>

CUDPNet::CUDPNet(CIMediator * pme)
{
	m_pme = pme;
	m_socket = NULL;
	m_hthread = NULL;
	m_bflagquit = false;
}

CUDPNet::~CUDPNet()
{

}

bool CUDPNet::Initial()
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
	m_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(m_socket == INVALID_SOCKET)
	{
		UnInitial();
		return false;
	}

	//绑定
	sockaddr_in sockaddrserve;
	sockaddrserve.sin_family = AF_INET;
	sockaddrserve.sin_port = htons(DEAFAULT_UDP_PORT);
	sockaddrserve.sin_addr.s_addr = CINet::GetHostIp();

	int nsize = sizeof(sockaddr_in);
	if(SOCKET_ERROR == bind(m_socket,(const sockaddr *)&sockaddrserve,nsize))
	{
		closesocket(m_socket);
		WSACleanup();
		return false;
	}

	bool optval = true;
	setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,(const char *)&optval,sizeof(bool));

	this->m_hthread = (HANDLE)_beginthreadex(NULL,0,&ThreadRecv,this,0,NULL);
	this->m_bflagquit = true;

	return true;
}

unsigned __stdcall CUDPNet::ThreadRecv( void * lpvoid)
{
	CUDPNet * pthis = (CUDPNet *)lpvoid;

	char szbuf[DEAFAULT_SIZE] = {0};
	sockaddr_in frominfo;
	int nsize = sizeof(sockaddr_in);
	while(pthis->m_bflagquit)
	{
		int res = recvfrom(pthis->m_socket,szbuf,DEAFAULT_SIZE,0,(sockaddr *)&frominfo,&nsize);
		
		if(res > 0)
		{
			//交给
			pthis->m_pme->DealData(&frominfo,szbuf);
		}
	}

	return 0;
}

void CUDPNet::UnInitial()
{
	WSACleanup();

	if(m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}
}

bool CUDPNet::Send(sockaddr_in *addr,char * szbuf,int len)
{
	if(0 >= sendto(m_socket,szbuf,len,0,(const sockaddr *)addr,sizeof(sockaddr_in)))
	{
		return false;
	}

	return true;
}
