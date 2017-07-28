#pragma once 
#include "IMediator.h"
#include "UDPNet.h"
#include "TCPNet.h"
#include "MyDecodevideo.h"

class CABsIMediator : public CIMediator
{
public:
	CABsIMediator();
	~CABsIMediator();
public:
	bool Open(HWND hwnd);
	void Close();
	bool SendData(char *szbuf,int len);
	bool DealData(char *szbuf);
	void SetServerAddr(long IP);
	bool DealTCPFile();
public:
	CUDPNet * m_udp;
	CTCPNet * m_tcp;
	HWND m_hwnd;
public:
	CMyDecodevideo * m_mydecoed;
};
