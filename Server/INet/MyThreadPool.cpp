#include "MyThreadPool.h"
#include <iostream>
#include <process.h>

using namespace MyThreadPool;

CMyThreadPool::CMyThreadPool(void)
{
	m_Semaphore = NULL;
	m_maxthreadnum = 0;
	m_Createthreadnum = 0;
	m_bflagquit = false;
	m_runthreadnum = 0;
	InitializeCriticalSection(&cs_qu);
}

CMyThreadPool::~CMyThreadPool(void)
{
}

bool CMyThreadPool::CreateThreadPool(long lminthreadnum,long lmaxthreadnum)
{
	//校验参数
	if(lminthreadnum <= 0 || lmaxthreadnum < lminthreadnum)
	{
		return false;
	}
	//创建线程池
	m_bflagquit = true;
	this->m_maxthreadnum = lmaxthreadnum;
	this->m_Createthreadnum = lminthreadnum;
	
	this->m_Semaphore = CreateSemaphore(NULL,0,lmaxthreadnum,NULL);
	if(m_Semaphore == NULL)
	{
		return false;
	}

	for(long i=0;i<lminthreadnum;i++)
	{
		HANDLE hThread = (HANDLE)_beginthreadex(NULL,0,&MyThreadPool,this,0,NULL);
		if(hThread)
		{
			this->m_lstThread.push_back(hThread);
		}
	}

	return true;
}

void CMyThreadPool::Destroythreadpool()
{
	m_bflagquit = false;
	ReleaseSemaphore(m_Semaphore,this->m_Createthreadnum,0);
	Sleep(100);

	std::list<HANDLE>::iterator ite = this->m_lstThread.begin();
	while(ite != this->m_lstThread.end())
	{
		if(WAIT_TIMEOUT == WaitForSingleObject(*ite,100))
		{
			TerminateThread(*ite,-1);
		}
		CloseHandle(*ite);
		*ite = NULL;
		ite++;
	}
	m_lstThread.clear();

	ITask * pitask = NULL;
	while(m_qItask.empty() == false)
	{
		pitask = m_qItask.front();
		m_qItask.pop();
		delete pitask;
		pitask = NULL;
	}

	if(m_Semaphore != NULL)
	{
		CloseHandle(m_Semaphore);
		m_Semaphore = NULL;
	}

	DeleteCriticalSection(&cs_qu);
}

unsigned __stdcall CMyThreadPool::MyThreadPool( void * lpvoid)
{
	CMyThreadPool * pthis = (CMyThreadPool *)lpvoid;
	if(!pthis->ThreadProc())
	{
		std::cout<<"CMyThreadPool::ThreadProc failed"<<std::endl;
	}
	
	return 0;
}

bool CMyThreadPool::ThreadProc()
{
	ITask * pitask = NULL;
	while(m_bflagquit)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_Semaphore,INFINITE))
		{
			if(m_bflagquit == false)
			{
				break;
			}
			InterlockedIncrement((long *)&m_runthreadnum);
			
			while(m_qItask.empty() == false)
			{
				EnterCriticalSection(&cs_qu);
				if(m_qItask.empty() == false)
				{
					pitask = m_qItask.front();
					m_qItask.pop();
				}
				else
				{
					LeaveCriticalSection(&cs_qu);
					break;
				}
				LeaveCriticalSection(&cs_qu);

				pitask->RunITask();
				delete pitask;
				pitask = NULL;
			}
			InterlockedDecrement((long *)&m_runthreadnum);
		}
	}
	return true;
}

bool CMyThreadPool::PushItask(ITask * task)
{
	if(task == NULL)
	{
		return false;
	}

	EnterCriticalSection(&cs_qu);
	m_qItask.push(task);
	LeaveCriticalSection(&cs_qu);

	//释放信号量
	if(m_runthreadnum < m_Createthreadnum)
	{
		ReleaseSemaphore(m_Semaphore,1,NULL);
	}
	else if(m_Createthreadnum < m_maxthreadnum)
	{
		HANDLE hthread = (HANDLE)_beginthreadex(NULL,0,&MyThreadPool,this,0,NULL);
		if(hthread)
		{
			m_lstThread.push_back(hthread);
			m_Createthreadnum++;
		}
		ReleaseSemaphore(m_Semaphore,1,NULL);
	}
	else
	{

	}

	return true;
}