#pragma once 
#include "IMediator.h"
#include "UDPNet.h"
#include "TCPNet.h"
#include "Myffmpeg.h"

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
public:
	CUDPNet * m_udp;
	CTCPNet * m_tcp;
	HWND m_hwnd;
	CMyffmpeg * m_myffmpeg;
};
