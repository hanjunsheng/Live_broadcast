#pragma once
#ifndef MYTHREADPOOL_H_
#define MYTHREADPOOL_H_

#include <queue>
#include <list>
#include <Windows.h>

namespace MyThreadPool
{
	class ITask
	{
	public:
		ITask(){}
		virtual ~ITask(){}
	public:
		virtual void RunITask() = 0;
	};

	class CMyThreadPool
	{
	public:
		CMyThreadPool(void);
		~CMyThreadPool(void);
	public:
		//1.创建线程池
		bool CreateThreadPool(long lminthreadnum,long lmaxthreadnum);
		//2.销毁线程池
		void Destroythreadpool();
		//3.线程函数
		static unsigned __stdcall MyThreadPool( void * );
		bool ThreadProc();
		//4.投递任务
		bool PushItask(ITask * task);
	public:
		std::queue<ITask *> m_qItask;
		HANDLE  m_Semaphore;
		std::list<HANDLE> m_lstThread;
		long m_maxthreadnum;
		long m_Createthreadnum;
		long m_runthreadnum;
		bool m_bflagquit;
		CRITICAL_SECTION cs_qu;
	};
};
#endif
