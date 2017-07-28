#include "AbsIMediator.h"
#include "MyThreadPool.h"
#include <iostream>
using namespace std;
using namespace MyThreadPool;

//测试变量
int a = 0;
int b = 0;

//----------------------------------------------------------------------------------------
class CMyFileTask : public ITask
{
public:
	CMyFileTask(STRU_REMARCHORINFO * pachor,STRU_REMAUDIENCEINFO * paudience)
	{
		this->m_parchor = pachor;
		this->m_paudience = paudience;
	}
	~CMyFileTask(){}
public:
	void RunITask();
private:
	STRU_REMARCHORINFO * m_parchor;
	STRU_REMAUDIENCEINFO * m_paudience;
};

void CMyFileTask::RunITask()
{
	HANDLE hevent[] = {m_parchor->FileSemaphore,m_paudience->bflagquit};
	DWORD res;
	while(1)
	{
		res = WaitForMultipleObjects(2,hevent,FALSE,INFINITE);
		
		if(res == WAIT_OBJECT_0 + 0)
		{
			send(m_paudience->socket,m_parchor->pszbuf + m_paudience->sendoff * DEFPACK_SIZE,m_parchor->dwNumberOfBytesRecvd,0);
			if(m_paudience->sendoff == 49)
			{
				m_paudience->sendoff = 0;
			}
			else
			{
				m_paudience->sendoff++;
			}
			b++;
			int *p = (int*)m_parchor->pszbuf;
			cout<<"send:"<<b<<" type "<<*p<<endl;
		}

		if(res == WAIT_OBJECT_0 + 1)
		{
			break;
		}
	}
}

//-------------------------------------------------------------------------------------------

CABsIMediator::CABsIMediator()
{
	m_udp = new CUDPNet(this);
	m_tcp = new CTCPNet(this);
}

CABsIMediator::~CABsIMediator()
{
	if(m_udp)
	{
		delete m_udp;
		m_udp = NULL;
	}

	if(m_tcp)
	{
		delete m_tcp;
		m_tcp = NULL;
	}
}

void CABsIMediator::ShowHostIP()
{
	in_addr addr;
	addr.S_un.S_addr = CINet::GetHostIp();

	cout<<"本服务器IP地址为"<<inet_ntoa(addr)<<endl;
}


bool CABsIMediator::Open()
{
	//初始化UDP
	if(!m_udp->Initial())
	{
		return false;
	}

	//初始化TCP
	if(!m_tcp->Initial())
	{
		return false;
	}

	//加载数据库
	m_ado.OpenDateBase(_T("zhibo"),DATEBASE_TYPE_SQL2005,_T("sa"),_T("sa"),_T("."));

	//先显示服务器IP
	this->ShowHostIP();

	return true;
}

void CABsIMediator::Close()
{
	//清空链表
	list<STRU_EACHJUSTACCEPT *>::iterator itejust = this->m_lstjustaccpet.begin();
	while(itejust != this->m_lstjustaccpet.end())
	{
		delete *itejust;
		*itejust = NULL;
		++itejust;
	}

	list<STRU_REMARCHORINFO *>::iterator itearchor = this->m_lstahchor.begin();
	while(itearchor != this->m_lstahchor.end())
	{
		(*itearchor)->mythreadpool.Destroythreadpool();
		CloseHandle((*itearchor)->FileSemaphore);
		(*itearchor)->FileSemaphore = NULL;
		closesocket((*itearchor)->socket);
		(*itearchor)->socket = NULL;
		delete (*itearchor);
		*itearchor = NULL;
		++itearchor;
	}

	list<STRU_REMAUDIENCEINFO *>::iterator itecudience = this->m_lstaudience.begin();
	while(itecudience != this->m_lstaudience.end())
	{
		SetEvent((*itecudience)->bflagquit);
		closesocket((*itecudience)->socket);
		(*itecudience)->socket = NULL;
		delete *itecudience;
		*itecudience = NULL;
		++itecudience;
	}

	m_udp->UnInitial();
	m_tcp->UnInitial();
}

bool CABsIMediator::SendTCPData(char *szbuf,int len)
{
	if(!m_tcp->Send(NULL,szbuf,len))
	{
		return false;
	}
	return true;
}

bool CABsIMediator::DealTCPData(MYSOCKETEX * pmysock,char *szbuf)
{
	int rc = WSAWaitForMultipleEvents(1,&pmysock->wv.hEvent,TRUE,INFINITE,TRUE);
	if (rc == WSA_WAIT_FAILED) 
	{
		wprintf(L"WSAWaitForMultipleEvents failed with error: %d\n", WSAGetLastError());
		return false;
	}

	DWORD * pdwNum = NULL;
	STRU_REMARCHORINFO * sr = NULL;
	list<STRU_REMARCHORINFO *>::iterator ite = this->m_lstahchor.begin();
	while(ite != this->m_lstahchor.end())
	{
		if(pmysock->socket == (*ite)->socket)
		{
			memcpy((*ite)->pszbuf + (*ite)->readoff * DEFPACK_SIZE,szbuf,DEFPACK_SIZE);
			pdwNum = &((*ite)->dwNumberOfBytesRecvd);
			if((*ite)->readoff == 49)
			{
				(*ite)->readoff = 0;
			}
			else
			{
				(*ite)->readoff++;
			}
			sr = (*ite);
			break;
		}
		++ite;
	}

	DWORD bflag = 0;
	rc = WSAGetOverlappedResult(pmysock->socket,&pmysock->wv,pdwNum,FALSE,&bflag);
	if (rc == FALSE) 
	{
		wprintf(L"WSARecv operation failed with error: %d\n", WSAGetLastError());
		return false;
	}
	a++;
	int *p = (int *)szbuf;
	cout<<"read:"<<*pdwNum<<" "<<a<<" "<<*p<<endl;

	WSAResetEvent(pmysock->wv.hEvent);

	ReleaseSemaphore(sr->FileSemaphore,sr->mythreadpool.m_runthreadnum,NULL);		
						
	return true;
}

bool CABsIMediator::SendData(sockaddr_in *addr,char *szbuf,int len)
{
	if(!m_udp->Send(addr,szbuf,len))
	{
		return false;
	}
	return true;
}

bool CABsIMediator::DealData(sockaddr_in *addr,char *szbuf)
{
	int *ptype = (int *)szbuf;

	switch(*ptype)
	{
	case PT_LOGIN_RQ:
		{
			cout<<"收到1条登陆消息"<<endl;
			this->DealLoginRQ(addr,szbuf);
		}
		break;
	case PT_REGISTER_QS:
		{
			cout<<"收到1条注册消息"<<endl;
			this->DealRegisterRQ(addr,szbuf);
		}
		break;
	case PT_SELECT_QS:
		{
			cout<<"收到1条选择消息"<<endl;
			this->DealSelectRQ(addr,szbuf);
		}
		break;
	case PT_CHATMSG_INFO:
		{
			if(addr->sin_addr.s_addr == CINet::GetHostIp())
			{
				break;
			}
			cout<<"收到1条聊天消息"<<endl;
			this->DealChatMSG(szbuf);
		}
		break;
	case PT_QUIT_NTF:
		{
			if(addr->sin_addr.s_addr == CINet::GetHostIp())
			{
				break;
			}
			cout<<"收到1条退出消息"<<endl;
			this->DealQuitNTF(szbuf);
		}
		break;
	case PT_ARCHOR_INFO:
		{
			if(addr->sin_addr.s_addr == CINet::GetHostIp())
			{
				break;
			}
			cout<<"收到1条房间标题消息"<<endl;
			this->DealArchorInfo(szbuf);
		}
		break;
	case PT_GIFT_RQ:
		{
			if(addr->sin_addr.s_addr == CINet::GetHostIp())
			{
				break;
			}
			cout<<"收到1条礼物消息"<<endl;
			this->DealGiftMSG(szbuf);
		}
		break;
	case PT_FAVORITE_INFO:
		{
			cout<<"收到1条关注消息"<<endl;
			this->DealFavoriteMSG(addr,szbuf);
		}
		break;
	case PT_KAIBONTF:
		{
			if(addr->sin_addr.s_addr == CINet::GetHostIp())
			{
				break;
			}
			cout<<"收到1条开播提醒消息"<<endl;
			this->DealKaiBoNTF(szbuf);
		}
		break;
	case PT_RETURN_RQ:
		{
			cout<<"收到1条返回消息"<<endl;
			this->DealReturnMSG(szbuf);
		}
		break;
	}
	return true;
}

void CABsIMediator::DealReturnMSG(char * szbuf)
{
	STRU_RETURNRQ * sr = (STRU_RETURNRQ *)szbuf;

	list<STRU_REMAUDIENCEINFO *>::iterator ite = this->m_lstaudience.begin();
	while(ite != this->m_lstaudience.end())
	{
		if(0 == wcscmp((*ite)->secondname,sr->myname))
		{
			SetEvent((*ite)->bflagquit);
			break;
		}
		++ite;
	}
}

void CABsIMediator::DealLoginRQ(sockaddr_in *addr,char *szbuf)
{
	STRU_LOGININFO *sl = (STRU_LOGININFO *)szbuf;

	STRU_PERSONINFO sp;
	if(this->SearchSQL(sl->name,sl->password,sp.secondname))
	{
		sp.type = PT_PERSON_INFO;
		this->SendData(addr,(char *)&sp,sizeof(STRU_PERSONINFO));
		cout<<"发送1条个人信息消息"<<endl;

		STRU_RSYESNO sr;
		sr.type = PT_LOGIN_YESNO;
		sr.rs = RS_YES;
		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条登陆回复消息"<<endl;
		
		if(sl->logintype == TY_AUDIENCE)
		{
			//观众信息
			in_addr inaddr;
			inaddr.S_un.S_addr = addr->sin_addr.S_un.S_addr;
			char *paddr = inet_ntoa(inaddr);
			TCHAR ipwide[IPADDRESS_SIZE] = {0};
			MultiByteToWideChar(CP_OEMCP,NULL,paddr,-1,ipwide,IPADDRESS_SIZE);

			list<STRU_EACHJUSTACCEPT *>::iterator itejust = this->m_lstjustaccpet.begin();
			while(itejust != this->m_lstjustaccpet.end())
			{
				if(0 == wcscmp((*itejust)->ip,ipwide))
				{
					STRU_REMAUDIENCEINFO * paudience = new STRU_REMAUDIENCEINFO;
					wcscpy_s(paudience->secondname,USERSECONDNAME_SIZE,sp.secondname);
					wcscpy_s(paudience->ip,IPADDRESS_SIZE,ipwide);
					paudience->socket = (*itejust)->socket;
					paudience->sendoff = 0;
					paudience->myfiletask = NULL;
					paudience->bflagquit = CreateEvent(NULL,FALSE,FALSE,NULL);

					this->m_lstaudience.push_back(paudience);

					delete *itejust;
					*itejust = NULL;
					this->m_lstjustaccpet.erase(itejust);
					break;
				}
				++itejust;
			}

			//发送给观众主播数据
			list<STRU_REMARCHORINFO *>::iterator ite = this->m_lstahchor.begin();
			while(ite != this->m_lstahchor.end())
			{
				STRU_SELECTALLINFO all;
				all.type = PT_SELECT_ALL;
				wcscpy_s(all.secondname,USERSECONDNAME_SIZE,(*ite)->secondname);
				wcscpy_s(all.ip,IPADDRESS_SIZE,(*ite)->ip);
				this->SendData(addr,(char *)&all,sizeof(STRU_SELECTALLINFO));
				cout<<"发送1条选择消息"<<endl;
				++ite;
			}

			//发给观众所关注的数据
			list<TCHAR *> favoritevalue;
			if(true == this->SerachAllFavoriteSQL(sp.secondname,favoritevalue))
			{
				list<TCHAR *>::iterator itefavorite = favoritevalue.begin();
				while(itefavorite != favoritevalue.end())
				{
					STRU_FAVORITEINFO sf;
					sf.type = PT_FAVORITE_INFO;
					wcscpy_s(sf.myname,USERSECONDNAME_SIZE,sp.secondname);
					wcscpy_s(sf.archorname,USERSECONDNAME_SIZE,(*itefavorite));
					this->SendData(addr,(char *)&sf,sizeof(STRU_FAVORITEINFO));
					cout<<"发送1条关注消息"<<endl;
					++itefavorite;
				}
			}
		}
		if(sl->logintype == TY_ARCHOR)
		{
			//主播信息
			in_addr inaddr;
			inaddr.S_un.S_addr = addr->sin_addr.S_un.S_addr;
			char *paddr = inet_ntoa(inaddr);
			TCHAR ipwide[IPADDRESS_SIZE] = {0};
			MultiByteToWideChar(CP_OEMCP,NULL,paddr,-1,ipwide,IPADDRESS_SIZE);

			list<STRU_EACHJUSTACCEPT *>::iterator itejust = this->m_lstjustaccpet.begin();
			while(itejust != this->m_lstjustaccpet.end())
			{
				if(0 == wcscmp((*itejust)->ip,ipwide))
				{
					STRU_REMARCHORINFO * parchor = new STRU_REMARCHORINFO;
					wcscpy_s(parchor->secondname,USERSECONDNAME_SIZE,sp.secondname);
					wcscpy_s(parchor->ip,IPADDRESS_SIZE,ipwide);
					parchor->socket = (*itejust)->socket;
					parchor->FileSemaphore = CreateSemaphore(NULL,0,3,NULL);
					parchor->mythreadpool.CreateThreadPool(1,3);
					parchor->readoff = 0;
					parchor->sendoff = 0;
					ZeroMemory(parchor->pszbuf,ARCHORSZBUF_SIZE);

					this->m_lstahchor.push_back(parchor);

					delete *itejust;
					*itejust = NULL;
					this->m_lstjustaccpet.erase(itejust);
					break;
				}
				++itejust;
			}

			//发送给主播 主播数据
			STRU_ARCHORINFO sa;
			if(this->SelectSearchSQL(sp.secondname,sa.title))
			{
				sa.type = PT_ARCHOR_INFO;
				this->SendData(addr,(char *)&sa,sizeof(STRU_ARCHORINFO));
				cout<<"发送1条房间信息消息"<<endl;
			}
	
			//发送给观众主播数据
			sockaddr_in addr_broard;
			addr_broard.sin_family = AF_INET;
			addr_broard.sin_port = htons(DEAFAULT_UDP_PORT);
			addr_broard.sin_addr.s_addr = INADDR_BROADCAST;
			STRU_SELECTALLINFO all;
			all.type = PT_SELECT_ALL;
			wcscpy_s(all.secondname,USERSECONDNAME_SIZE,sp.secondname);
			wcscpy_s(all.ip,IPADDRESS_SIZE,ipwide);
			if(this->SendData(&addr_broard,(char *)&all,sizeof(STRU_SELECTALLINFO)))
			{
				cout<<"发送1条选择消息"<<endl;
			}
			else
			{
				cout<<"发送1条选择消息error"<<endl;
			}
		}	
	}
	else
	{
		STRU_RSYESNO sr;
		sr.type = PT_LOGIN_YESNO;
		sr.rs = RS_NO;
		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条登陆回复消息"<<endl;
	}
}

void CABsIMediator::DealRegisterRQ(sockaddr_in *addr,char *szbuf)
{
	STRU_REGISTERINFO * sr = (STRU_REGISTERINFO *)szbuf;

	if(this->RegisSearchSQL(sr->registype,sr->name,sr->password,sr->secondname))
	{
		STRU_RSYESNO sr;
		sr.type = PT_REGISTER_YESNO;
		sr.rs = RS_YES;
		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条注册回复消息"<<endl;
	}
	else
	{
		STRU_RSYESNO sr;
		sr.type = PT_REGISTER_YESNO;
		sr.rs = RS_NO;
		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条注册回复消息"<<endl;
	}
}

void CABsIMediator::DealSelectRQ(sockaddr_in *addr,char *szbuf)
{
	STRU_SELECTINFO *ss = (STRU_SELECTINFO *)szbuf;
	STRU_ARCHORINFO sa;

	//找到相应的主播
	STRU_REMARCHORINFO * par = NULL;
	list<STRU_REMARCHORINFO *>::iterator itearchor = this->m_lstahchor.begin();
	while(itearchor != this->m_lstahchor.end())
	{
		if(0 == wcscmp(ss->ip,(*itearchor)->ip))
		{
			par = *itearchor;
			break;
		}
		++itearchor;
	}

	//确认自己
	STRU_REMAUDIENCEINFO * pau = NULL;
	list<STRU_REMAUDIENCEINFO *>::iterator iteaudience = this->m_lstaudience.begin();
	while(iteaudience != this->m_lstaudience.end())
	{
		if(0 == wcscmp(ss->secondname,(*iteaudience)->secondname))
		{
			pau = *iteaudience;
			break;
		}
		++iteaudience;
	}

	if(par != NULL && pau != NULL)
	{
		CMyFileTask * myfiletask = new CMyFileTask(par,pau);
		pau->myfiletask = myfiletask;
		par->mythreadpool.PushItask(myfiletask);
	}

	if(this->SelectSearchSQL(par->secondname,sa.title))
	{
		//发送相关信息
		wcscpy_s(sa.second,USERSECONDNAME_SIZE,par->secondname);
		sa.type = PT_ARCHOR_INFO;
		this->SendData(addr,(char *)&sa,sizeof(STRU_ARCHORINFO));
		cout<<"发送1条房间信息消息"<<endl;

		STRU_RSYESNO sr;
		sr.type = PT_SELECT_YESNO;
		sr.rs = RS_YES;
		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条选择回复消息"<<endl;
	}
	else
	{
		STRU_RSYESNO sr;
		sr.type = PT_SELECT_YESNO;
		sr.rs = RS_NO;
		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条选择回复消息"<<endl;
	}
}

void CABsIMediator::DealChatMSG(char *szbuf)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEAFAULT_UDP_PORT);
	addr.sin_addr.s_addr = INADDR_BROADCAST;

	this->SendData(&addr,szbuf,sizeof(STRU_CHATINFO));
	cout<<"广播1条聊天消息"<<endl;
}

void CABsIMediator::DealKaiBoNTF(char *szbuf)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEAFAULT_UDP_PORT);
	addr.sin_addr.s_addr = INADDR_BROADCAST;

	this->SendData(&addr,szbuf,sizeof(STRU_KAIBOINFO));
	cout<<"广播1条开播提醒消息"<<endl;
}

void CABsIMediator::DealGiftMSG(char *szbuf)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEAFAULT_UDP_PORT);
	addr.sin_addr.s_addr = INADDR_BROADCAST;

	this->SendData(&addr,szbuf,sizeof(STRU_GIFTINFO));
	cout<<"广播1条礼物消息"<<endl;
}

void CABsIMediator::DealQuitNTF(char *szbuf)
{
	STRU_QUITNTF * sqn = (STRU_QUITNTF *)szbuf;
	if(sqn->logintype == TY_ARCHOR)
	{
		//发送广播退出
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(DEAFAULT_UDP_PORT);
		addr.sin_addr.s_addr = INADDR_BROADCAST;

		this->SendData(&addr,szbuf,sizeof(STRU_QUITNTF));

		//删除链表中的记录
		list<STRU_REMARCHORINFO *>::iterator ite = this->m_lstahchor.begin();
		while(ite != this->m_lstahchor.end())
		{
			if(0 == wcscmp((*ite)->secondname,sqn->secondname))
			{
				(*ite)->mythreadpool.Destroythreadpool();
				CloseHandle((*ite)->FileSemaphore);
				(*ite)->FileSemaphore = NULL;
				closesocket((*ite)->socket);
				(*ite)->socket = NULL;
				delete (*ite);
				*ite = NULL;
				this->m_lstahchor.erase(ite);
				break;
			}
			++ite;
		}
	}
	if(sqn->logintype == TY_AUDIENCE)
	{
		//删除链表中的记录
		list<STRU_REMAUDIENCEINFO *>::iterator ite = this->m_lstaudience.begin();
		while(ite != this->m_lstaudience.end())
		{
			if(0 == wcscmp((*ite)->secondname,sqn->secondname))
			{
				//设置任务退出事件为有信号
				SetEvent((*ite)->bflagquit);

				closesocket((*ite)->socket);
				(*ite)->socket = NULL;

				delete (*ite);
				*ite = NULL;
				this->m_lstaudience.erase(ite);
				break;
			}
			++ite;
		}
	}
}

void CABsIMediator::DealArchorInfo(char *szbuf)
{
	STRU_ARCHORINFO * sa = (STRU_ARCHORINFO *)szbuf;

	//更改数据库
	this->SetTitle(sa->second,sa->title);

	//发送广播 更改标题
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(DEAFAULT_UDP_PORT);
	addr.sin_addr.s_addr = INADDR_BROADCAST;

	this->SendData(&addr,szbuf,sizeof(STRU_ARCHORINFO));
	cout<<"广播1条房间标题消息"<<endl;
}

void CABsIMediator::DealFavoriteMSG(sockaddr_in *addr,char * szbuf)
{
	STRU_FAVORITEINFO * sf = (STRU_FAVORITEINFO *)szbuf;
	if(true == this->SerachFavoriteSQL(sf->myname,sf->archorname))
	{
		STRU_RSYESNO sr;
		sr.type = PT_FAVORITE_RS;
		sr.rs = RS_YES;

		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条关注回复消息"<<endl;
	}
	else
	{
		STRU_RSYESNO sr;
		sr.type = PT_FAVORITE_RS;
		sr.rs = RS_NO;

		this->SendData(addr,(char *)&sr,sizeof(STRU_RSYESNO));
		cout<<"发送1条关注回复消息"<<endl;
	}
}

bool CABsIMediator::SearchSQL(TCHAR * name,TCHAR *password,TCHAR *second)
{
	TCHAR strsql[SQLSTRLENGTH] = {0};
	wcscpy_s(strsql,SQLSTRLENGTH,_T("select userpassword, usersecond from userinfo where username = '"));
	wcscat_s(strsql,SQLSTRLENGTH,name);
	wcscat_s(strsql,SQLSTRLENGTH,_T("'"));
	list<TCHAR*> strQueryCloum;
	TCHAR columnpass[SQLCOLUMNLENGTH] = {0};
	TCHAR columnsecond[SQLCOLUMNLENGTH] = {0};
	wcscpy_s(columnpass,SQLCOLUMNLENGTH,_T("userpassword"));
	wcscpy_s(columnsecond,SQLCOLUMNLENGTH,_T("usersecond"));
	strQueryCloum.push_back(columnpass);
	strQueryCloum.push_back(columnsecond);
	list<TCHAR*> strDataValue;

	m_ado.GetData(strsql,strQueryCloum,strQueryCloum.size(),strDataValue);

	if(strDataValue.empty() == true)
	{
		return false;
	}

	if(0 == wcscmp(password,strDataValue.back()))
	{
		wcscpy_s(second,USERSECONDNAME_SIZE,strDataValue.front());
		return true;
	}
	
	return false;
}

bool CABsIMediator::RegisSearchSQL(LOGINTYPE type,TCHAR * name,TCHAR *password,TCHAR *secondname)
{
	//查找用户名是否存在------------------------------------------------------------------------------
	TCHAR strsqlname[SQLSTRLENGTH] = {0};
	wcscpy_s(strsqlname,SQLSTRLENGTH,_T("select userpassword, usersecond from userinfo where username = '"));
	wcscat_s(strsqlname,SQLSTRLENGTH,name);
	wcscat_s(strsqlname,SQLSTRLENGTH,_T("'"));
	list<TCHAR*> strQueryCloumname;
	TCHAR columnpassname[SQLCOLUMNLENGTH] = {0};
	wcscpy_s(columnpassname,SQLCOLUMNLENGTH,_T("userpassword"));
	strQueryCloumname.push_back(columnpassname);
	list<TCHAR*> strDataValuename;

	m_ado.GetData(strsqlname,strQueryCloumname,strQueryCloumname.size(),strDataValuename);
	
	if(strDataValuename.empty() == false)
	{
		return false;
	}

	//查找昵称是否存在------------------------------------------------------------------------------
	TCHAR strsqlsec[SQLSTRLENGTH] = {0};
	wcscpy_s(strsqlsec,SQLSTRLENGTH,_T("select userpassword from userinfo where usersecond = '"));
	wcscat_s(strsqlsec,SQLSTRLENGTH,secondname);
	wcscat_s(strsqlsec,SQLSTRLENGTH,_T("'"));
	list<TCHAR*> strQueryCloumsec;
	TCHAR columnpasssec[SQLCOLUMNLENGTH] = {0};
	wcscpy_s(columnpasssec,SQLCOLUMNLENGTH,_T("userpassword"));
	strQueryCloumsec.push_back(columnpasssec);
	list<TCHAR*> strDataValuesec;

	m_ado.GetData(strsqlsec,strQueryCloumsec,strQueryCloumsec.size(),strDataValuesec);
	
	if(strDataValuesec.empty() == false)
	{
		return false;
	}

	//--------------添加---------------------------------------------------------
	list<TCHAR*> strresiginfo;
	TCHAR regisname[USERNAME_SIZE] = {0};
	TCHAR regispass[PASSWORD_SIZE] = {0};
	TCHAR regissecond[USERSECONDNAME_SIZE] = {0};
	wcscpy_s(regisname,USERNAME_SIZE,name);
	wcscpy_s(regispass,PASSWORD_SIZE,password);
	wcscpy_s(regissecond,USERSECONDNAME_SIZE,secondname);
	strresiginfo.push_back(regisname);
	strresiginfo.push_back(regispass);
	strresiginfo.push_back(regissecond);

	m_ado.AddData(_T("userinfo"),strresiginfo,strresiginfo.size());

	//如果是主播 还要添加房间表信息
	if(type == TY_ARCHOR)
	{
		list<TCHAR*> strresigtitle;
		TCHAR archorname[USERSECONDNAME_SIZE] = {0};
		TCHAR title[ROOMTITLE_SIZE] = {0};
		wcscpy_s(archorname,USERSECONDNAME_SIZE,secondname);
		wcscpy_s(title,ROOMTITLE_SIZE,_T("精彩不断"));
		strresigtitle.push_back(archorname);
		strresigtitle.push_back(title);

		m_ado.AddData(_T("roomtitle"),strresigtitle,strresigtitle.size());
	}

	return true;
}

bool CABsIMediator::SelectSearchSQL(TCHAR * second,TCHAR *title)
{
	TCHAR strsql[SQLSTRLENGTH] = {0};
	wcscpy_s(strsql,SQLSTRLENGTH,_T("select roomtitle from roomtitle where username = '"));
	wcscat_s(strsql,SQLSTRLENGTH,second);
	wcscat_s(strsql,SQLSTRLENGTH,_T("'"));
	list<TCHAR*> strQueryCloum;
	TCHAR columntitle[SQLCOLUMNLENGTH] = {0};
	wcscpy_s(columntitle,SQLCOLUMNLENGTH,_T("roomtitle"));
	strQueryCloum.push_back(columntitle);
	list<TCHAR*> strDataValue;

	m_ado.GetData(strsql,strQueryCloum,strQueryCloum.size(),strDataValue);

	if(strDataValue.empty() == true)
	{
		return false;
	}

	wcscpy_s(title,ROOMTITLE_SIZE,strDataValue.front());

	return true;
}

void CABsIMediator::SetTitle(TCHAR * second,TCHAR *title)
{
	TCHAR strsql[SQLSTRLENGTH] = {0};
	wcscpy_s(strsql,SQLSTRLENGTH,_T("select roomtitle from roomtitle where username = '"));
	wcscat_s(strsql,SQLSTRLENGTH,second);
	wcscat_s(strsql,SQLSTRLENGTH,_T("'"));
	list<TCHAR*> strQueryCloum;
	TCHAR columntitle[SQLCOLUMNLENGTH] = {0};
	wcscpy_s(columntitle,SQLCOLUMNLENGTH,_T("roomtitle"));
	strQueryCloum.push_back(columntitle);
	list<TCHAR*> strDataValue;
	TCHAR editroontitle[ROOMTITLE_SIZE] = {0};
	wcscpy_s(editroontitle,ROOMTITLE_SIZE,title);
	strDataValue.push_back(editroontitle);

	m_ado.EditData(strsql,strQueryCloum.size(),strQueryCloum,strDataValue);
}

void CABsIMediator::DealTCPaccept(sockaddr_in *addr,SOCKET sock,char * ps)
{
	STRU_EACHJUSTACCEPT * se = new STRU_EACHJUSTACCEPT;
	se->socket = sock;

	in_addr inaddr;
	inaddr.S_un.S_addr = addr->sin_addr.s_addr;
	char *pip = inet_ntoa(inaddr);
	MultiByteToWideChar(CP_OEMCP,NULL,pip,-1,se->ip,IPADDRESS_SIZE);
	se->pszbuf = ps;

	this->m_lstjustaccpet.push_back(se);
}

bool CABsIMediator::SerachFavoriteSQL(TCHAR * myname,TCHAR * archorname)
{
	list<TCHAR*> strfavorite;
	TCHAR gmyname[USERNAME_SIZE] = {0};
	TCHAR garchorname[USERSECONDNAME_SIZE] = {0};
	wcscpy_s(gmyname,USERNAME_SIZE,myname);
	wcscpy_s(garchorname,USERSECONDNAME_SIZE,archorname);
	strfavorite.push_back(gmyname);
	strfavorite.push_back(garchorname);

	m_ado.AddData(_T("favorite"),strfavorite,strfavorite.size());

	return true;
}

bool CABsIMediator::SerachAllFavoriteSQL(TCHAR * myname,list<TCHAR *>& strvalue)
{
	TCHAR strsqlsec[SQLSTRLENGTH] = {0};
	wcscpy_s(strsqlsec,SQLSTRLENGTH,_T("select archorname from favorite where username = '"));
	wcscat_s(strsqlsec,SQLSTRLENGTH,myname);
	wcscat_s(strsqlsec,SQLSTRLENGTH,_T("'"));
	list<TCHAR*> strQueryCloum;
	TCHAR archorname[SQLCOLUMNLENGTH] = {0};
	wcscpy_s(archorname,SQLCOLUMNLENGTH,_T("archorname"));
	strQueryCloum.push_back(archorname);

	m_ado.GetData(strsqlsec,strQueryCloum,strQueryCloum.size(),strvalue);

	return true;
}

void CABsIMediator::Run()
{
	int endnum = 0;
	while(1)
	{
		cin>>endnum;
		if(endnum == -1)
		{
			break;
		}
		cout<<"输入命令无效，输入-1关闭该服务器"<<endl;
	}
}