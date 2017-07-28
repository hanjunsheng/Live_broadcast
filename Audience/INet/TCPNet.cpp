#include "stdafx.h"
#include "TCPNet.h"
#include <process.h>
#include "AbsIMediator.h"
#include <fstream>
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
	/*else
	{*/
		AfxGetMainWnd()->MessageBox(_T("连接服务器成功！"));
		this->m_bflagquit = true;
		this->m_hthread = (HANDLE)_beginthreadex(NULL,0,&ThreadRecv,this,0,NULL);
		
		return true;
	/*}*/
}

unsigned __stdcall CTCPNet::ThreadRecv( void * lpvoid)
{
	CTCPNet * pthis = (CTCPNet *)lpvoid;

	while(pthis->m_bflagquit)
	{
		if(pthis->ReceiveFile() == true)
		{
			//交给
			((CABsIMediator *)pthis->m_pme)->DealTCPFile();
		}
	}

	return 0;
}

void CTCPNet::UnInitial()
{
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

	WSACleanup();

	if(m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
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

bool  CTCPNet::ReceiveFile()
{
	char szbuf[1032] = {0};
	int nfilesize = 0;
	int res = recv(this->m_socket,szbuf,1032,0);
	if(res <= 0)
	{
		return false;
	}

	int offhead = res;
	while(res < 1032)
	{
		res = recv(this->m_socket,szbuf + offhead,1032 - offhead,0);
		res += offhead;
		offhead = res;
	}

	STRU_DATASIZEINFO * psd = (STRU_DATASIZEINFO *)szbuf;
	TRACE("%d",psd->type);
	if(psd->type == FT_FILESIZE)
	{
		nfilesize = psd->filesize;
	}

	if(nfilesize == 0)
		return false;

	fstream fs;
	fs.open("../TempFile/src01.h264",fstream::binary|fstream::out|fstream::trunc);
	if(!fs)
	{
		AfxGetMainWnd()->MessageBox(_T("file created failed"));
		return false;
	}

	while(nfilesize)
	{
		if(nfilesize < 0)
		{
			return false;
		}
		ZeroMemory(szbuf,1032);
		int nreadnum = recv(this->m_socket,szbuf,1032,0);
		int offdata = nreadnum;

		while(nreadnum < 1032)
		{
			nreadnum = recv(this->m_socket,szbuf + offdata,1032 - offdata,0);
			nreadnum += offdata;
			offdata = nreadnum;
		}

		TRACE("%d\n",nreadnum);
		
		STRU_DATAINFO * psdi = (STRU_DATAINFO *)szbuf;
		if(psdi->type == FT_FILEDATA)
		{
			nfilesize -= psdi->filesize;
			fs.write(psdi->szbuf,psdi->filesize);
		}
		
		if(nfilesize < 1024)
		{
			int a = 5;
		}
	}

	fs.close();

	return true;
}

//bool CTCPNet::ReceiveFile()
//{
//	long filesize = 0;
//	int res = recv(this->m_socket,(char *)&filesize,sizeof(filesize),0);
//	
//	if(filesize <= 0)
//	{
//		return false;
//	}
//
//	fstream fs;
//
//	fs.open("../TempFile/src01.h264",fstream::binary|fstream::out|fstream::trunc);
//	
//	if(!fs)
//	{
//		AfxGetMainWnd()->MessageBox(_T("文件创建失败"));
//		return false;
//	}
//
//	char szbuf[1032] = {0};
//	while(filesize > 0)
//	{
//		ZeroMemory(szbuf,1032);
//		int nreadnum = recv(this->m_socket,szbuf,1032,0);
//		if(nreadnum > 0)
//		{
//			filesize -= nreadnum;
//			fs.write(szbuf,nreadnum);
//		}
//	}
//
//	fs.close();
//
//	return true;
//}