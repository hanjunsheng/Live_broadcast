#include "stdafx.h"
#include "UDPNet.h"
#include <process.h>
#include "MyWnd.h"

CUDPNet::CUDPNet(CIMediator * pme)
{
	m_pme = pme;
	m_bflagquit = false;
	m_socket = NULL;
	m_mywnd = NULL;
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

	//绑定bind
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = GetHostIp();
	addr.sin_port = htons(DEAFAULT_UDP_PORT);

	if(SOCKET_ERROR == bind(m_socket,(const sockaddr *)&addr,sizeof(sockaddr_in)))
	{
		UnInitial();
		return false;
	}

	bool optval = true;
	setsockopt(m_socket,SOL_SOCKET,SO_BROADCAST,(const char *)&optval,sizeof(bool));

	this->m_mywnd = CMyWnd::GetDX();
	if(!m_mywnd->Create(NULL,_T("MyWnd")))
	{
		UnInitial();
		return false;
	}
	this->m_mywnd->SetptrUDP(this);

	WSAAsyncSelect(m_socket,m_mywnd->m_hWnd,UM_NM_RECVMSG,FD_READ);

	return true;
}

void CUDPNet::RecvFrom(WPARAM wparam,LPARAM lparam)
{
	char szbuf[DEAFAULT_SIZE] = {0};

	int res = recvfrom(m_socket,szbuf,DEAFAULT_SIZE,0,NULL,NULL);
	if(res > 0)
	{
		//交给
		m_pme->DealData(szbuf);
	}
}

void CUDPNet::UnInitial()
{
	WSACleanup();

	if(m_socket)
	{
		closesocket(m_socket);
		m_socket = NULL;
	}

	CMyWnd::DestroyObject(this->m_mywnd);
}

bool CUDPNet::Send(char * szbuf,int len)
{
	if(0 >= sendto(m_socket,szbuf,len,0,(const sockaddr *)&this->m_serveraddress,sizeof(sockaddr_in)))
	{
		return false;
	}

	return true;
}

void CUDPNet::SetServerAddress(long IP)
{
	this->m_serveraddress.sin_family = AF_INET;
	this->m_serveraddress.sin_port = htons(DEAFAULT_UDP_PORT);
	this->m_serveraddress.sin_addr.s_addr = IP;
}