#pragma once
#include <WinSock2.h>
#include "packdef.h"
#pragma comment(lib,"Ws2_32.lib")

class CINet
{
public:
	CINet(){}
	virtual ~CINet(){}
public:
	virtual bool Initial() = 0;
	virtual void UnInitial() = 0;
	virtual bool Send(char * szbuf,int len) = 0;
	static long GetHostIp()
	{
		char szname[MAX_HOST_NAME] = {0};
		struct hostent* plist = NULL;
		struct in_addr addr;
		gethostname(szname,sizeof(szname));

		plist = gethostbyname(szname);
		if(plist->h_addr_list[0] != 0) 
		{
			addr.s_addr = *(u_long *) plist->h_addr_list[0];
            return addr.s_addr;
        }
		return 0;
	}
};