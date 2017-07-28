#include "stdafx.h"
#include "TCPNet.h"
#include <process.h>
#include <fstream>
#include "packdef.h"
using namespace std;

CTCPNet::CTCPNet(CIMediator * pme)
{
	m_pme = pme;
	m_hthread = NULL;
	m_bflagquit = false;
	m_socket = NULL;
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

	return true;
}

bool CTCPNet::Connect()
{
	if(SOCKET_ERROR == connect(m_socket,(const sockaddr*)&this->m_serveraddress,sizeof(sockaddr_in)))
	{
		AfxGetMainWnd()->MessageBox(_T("连接服务器失败！"));
		return false;
	}
	else
	{
		AfxGetMainWnd()->MessageBox(_T("连接服务器成功！"));
		this->m_hthread = (HANDLE)_beginthreadex(NULL,0,&ThreadRecv,this,0,NULL);
		this->m_bflagquit = true;

		return true;
	}
}

unsigned __stdcall CTCPNet::ThreadRecv( void * lpvoid)
{
	CTCPNet * pthis = (CTCPNet *)lpvoid;

	char szbuf[DEAFAULT_SIZE] = {0};
	while(pthis->m_bflagquit)
	{
		int res = recv(pthis->m_socket,szbuf,DEAFAULT_SIZE,0);
		
		if(res > 0)
		{
			//交给
			pthis->m_pme->DealData(szbuf);
		}
	}

	return 0;
}

void CTCPNet::UnInitial()
{
	WSACleanup();

	if(m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}

	if(m_hthread)
	{
		m_bflagquit = false;

		if(WAIT_TIMEOUT == WaitForSingleObject(m_hthread,100))
		{
			TerminateThread(m_hthread,-1);
		}

		CloseHandle(m_hthread);
		m_hthread = NULL;
	}
}

void CTCPNet::SetServerAddress(long IP)
{
	this->m_serveraddress.sin_family = AF_INET;
	this->m_serveraddress.sin_port = htons(DEAFAULT_TCP_PORT);
	this->m_serveraddress.sin_addr.s_addr = IP;
}

bool CTCPNet::Send(char * szbuf,int len)
{
	return false;
}

bool CTCPNet::sendfile()
{
	//打开文件
	fstream fs;
	fs.open("../TempFile/src01.h264",fstream::binary|fstream::in);
	if(!fs)
	{
		AfxGetMainWnd()->MessageBox(_T("打开文件失败"));
		return false;
	}

	fs.seekg(0,fstream::end);
	long nsize = (long)fs.tellg();
	fs.seekg(0,fstream::beg);

	//文件大小 
	STRU_DATASIZEINFO sds;
	sds.type = FT_FILESIZE;
	sds.filesize = nsize;
	DWORD sendfileheadsize = sizeof(STRU_DATASIZEINFO);

	int res = send(this->m_socket,(const char *)&sds,sendfileheadsize,0);
	if(res == SOCKET_ERROR)
	{
		AfxGetMainWnd()->MessageBox(_T("发送文件头失败"));
		return false;
	}

	//读文件
	while(1)
	{
		STRU_DATAINFO sd;
		sd.type = FT_FILEDATA;
		ZeroMemory(sd.szbuf,DEAFAULT_SIZE);

		fs.read(sd.szbuf,DEAFAULT_SIZE);
		int ncurrentsize = (int)fs.gcount();
		sd.filesize = ncurrentsize;

		if(ncurrentsize <= 0)
		{
			break;
		}
		else
		{
			int res = send(this->m_socket,(const char *)&sd,sendfileheadsize,0);
			if(res < 0)
			{
				AfxGetMainWnd()->MessageBox(_T("读取当前页失败"));
				return false;
			}
		}
	}

	//关闭文件
	fs.close();

	return true;
}

//bool CTCPNet::sendfile()
//{
//	//打开文件
//	fstream fs;
//	fs.open("../TempFile/src01.h264",fstream::binary|fstream::in);
//	if(!fs)
//	{
//		AfxGetMainWnd()->MessageBox(_T("打开文件失败"));
//		return false;
//	}
//
//	fs.seekg(0,fstream::end);
//	long nsize = (long)fs.tellg();
//	fs.seekg(0,fstream::beg);
//
//	//文件大小 
//	int filesize = nsize;
//
//	int res = send(this->m_socket,(const char *)&filesize,sizeof(filesize),0);
//	if(res == SOCKET_ERROR)
//	{
//		AfxGetMainWnd()->MessageBox(_T("发送文件头失败"));
//		return false;
//	}
//
//	//读文件
//	char szbuf[1032] = {0};
//	while(1)
//	{
//
//		fs.read(szbuf,1032);
//		int ncurrentsize = (int)fs.gcount();
//		if(ncurrentsize <= 0)
//		{
//			break;
//		}
//		else
//		{
//			int res = send(this->m_socket,szbuf,ncurrentsize,0);
//			if(res < 0)
//			{
//				AfxGetMainWnd()->MessageBox(_T("读取当前页失败"));
//				return false;
//			}
//		}
//	}
//
//	//关闭文件
//	fs.close();
//
//	return true;
//}
