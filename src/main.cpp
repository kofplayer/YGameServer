//
//  main.cpp
//  YGameServer
//
//  Created by 张锦 on 2017/8/12.
//  Copyright © 2017年 张锦. All rights reserved.
//

#include <iostream>
#include "./Common/Common.h"

using namespace YGAME_SERVER_NAMESPACE;

void onTaskCallEnd(bool isSucc, Task * pTask)
{
	LOG_DEBUG("onTaskCallEnd %d\n", isSucc);
}

class testTask : public Task {
public:
    testTask()
    {
        m_index = 0;
        m_isSucc = true;
    }
	virtual ~testTask()
	{
		m_index = 0;
		m_isSucc = 0;
	}
	void set_task_info(int32 i, bool isSucc)
	{
		m_index = i;
		m_isSucc = isSucc;
	}
protected:
    virtual void onStart()
	{
		LOG_DEBUG("onStart %d\n", m_index);
        doEnd(m_isSucc);
    }
    virtual void onEnd(bool isSucc)
	{
		LOG_DEBUG("onEnd %d, isSucc %d\n", m_index, isSucc);
    }
private:
    int32 m_index;
    bool m_isSucc;
};

class test_handler : public EventHandler {
public:
    test_handler()
    {
        pH = NULL;
    };
	virtual ~test_handler() {}
	void set_handler_info(int n)
	{
		m_n = n;
	}
    virtual void OnEvent(void * pParam, const EventFilter & filter)
	{
		LOG_DEBUG("OnEvent %d\n", m_n);
        if (pH) {
            gEvent->removeHandler(pH);
        }
    }
    test_handler * pH;
    int m_n;
};

class test_thread : public Thread
{
public:
	test_thread() : is_end(false)
	{
	}
	void set_id(int id)
	{
		aa = id;
	}
	virtual ~test_thread() {};

	bool is_end;
protected:
	virtual void run()
	{
		for (int i = 0; i < 10; ++i)
		{
			LOG_DEBUG("thread:%d run:%d\n", aa, i+1);
			Sleep(1000);
		}
		is_end = true;
	}
private:
	int aa;
};

class listen_thread : public Thread
{
public:
	virtual ~listen_thread() {};
protected:
	virtual void run()
	{
		NetListener * listener = gMemory.New<NetListener>();
		bool succ = listener->Create();
		succ = listener->Bind(INADDR_ANY, 9999);
		succ = listener->Listen();

		NetPConnect * conn = listener->Accept();
		if (conn)
		{
			LOG_DEBUG("Accept succ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
	}
private:
	int aa;
};

class listen_listener : public NetReadListener
{
public:
	virtual bool onNetRead(SOCKET_ID s)
	{

		NetPConnect * conn = m_listener->Accept();
		if (conn)
		{
			LOG_DEBUG("Accept succ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
		return true;
	}

public:
	NetListener * m_listener;
};

class poller_thread : public Thread
{
public:
	virtual ~poller_thread() {};
protected:
	virtual void run()
	{
		NetListener * listener = gMemory.New<NetListener>();
		bool succ = listener->Create();
		succ = listener->Bind(INADDR_ANY, 8888);
		succ = listener->Listen();

		NetSelectPoller * selectPolloer = gMemory.New<NetSelectPoller>();
		listen_listener * listenListener = gMemory.New<listen_listener>();
		listenListener->m_listener = listener;

		selectPolloer->addRead(listener->getSocket(), listenListener);
		while (true)
		{
			selectPolloer->waitEvent(1000);
		}
	}
private:
	int aa;
};


class test_coroutine : public Coroutine
{
public:
	virtual bool Run()
	{
		PT_BEGIN();
		work = 0;
		while (work < 10)
		{
			m_time = time(NULL);
			LOG_DEBUG("coroutine %d work %d\n", id, ++work);
			PT_WAIT_UNTIL(time(NULL) > m_time + 1);
		}
		PT_END();
	}

	int id;
	int work;
	time_t m_time;
};

int main(int argc, const char * argv[]) {

	LOG_ADD_WRITER(new ScreenLogWriter());

	FileLogWriter * pLog = new FileLogWriter();
	pLog->setLogLevel(LL_ERROR);
	pLog->setFilePreName("error_");
	LOG_ADD_WRITER(pLog);

	pLog = new FileLogWriter();
	pLog->setLogLevel(LL_DEBUG);
	LOG_ADD_WRITER(pLog);

	// 事件测试
// 	LOG_DEBUG("start event test\n");
//     test_handler * pH1 = gMemory.New<test_handler>();
// 	pH1->set_handler_info(1);
// 	test_handler * pH2 = gMemory.New<test_handler>();
// 	pH2->set_handler_info(2);
// 	test_handler * pH3 = gMemory.New<test_handler>();
// 	pH3->set_handler_info(3);
//     
//     pH1->pH = pH2;
//     pH2->pH = pH3;
//     
//     gEvent->addHandler(pH1, EventFilter(1));
//     gEvent->addHandler(pH2, EventFilter(1,2,3));
//     gEvent->addHandler(pH3, EventFilter(1,2,3));
//     gEvent->fire(EventFilter(1,2,3), NULL);
// 
// 	gMemory.Delete(pH1);
// 	gMemory.Delete(pH2);
// 	gMemory.Delete(pH3);
// 
// 	LOG_DEBUG("input any key to go next\n");
// 	getchar();


	// 事务测试
// 	LOG_DEBUG("start task test\n");
// 	gMemory.Init<testTask>(100, 10);
//     
//     testTask * task = gMemory.New<testTask>();
//     task->setCallBackFunc(onTaskCallEnd);
//     
//     testTask * task1 = gMemory.New<testTask>(); 
// 	task1->set_task_info(1, true);
//     task->addTask(task1);
//     
// 	testTask * task2 = gMemory.New<testTask>();
// 	task2->set_task_info(2, true);
//     task1->addTask(task2);
//     
// 	testTask * task3 = gMemory.New<testTask>();
// 	task3->set_task_info(3, true);
//     task2->addTask(task3);
//     
// 	testTask * task4 = gMemory.New<testTask>();
// 	task4->set_task_info(4, true);
//     task1->addTask(task4);
//     
// 	testTask * task5 = gMemory.New<testTask>();
// 	task5->set_task_info(5, true);
//     task3->addTask(task5);
//     
//     task->start();
//     
// 	gMemory.Delete(task);
// 	gMemory.Delete(task1);
// 	gMemory.Delete(task2);
// 	gMemory.Delete(task3);
// 	gMemory.Delete(task4);
// 	gMemory.Delete(task5);
// 	LOG_DEBUG("input any key to go next\n");
// 	getchar();

	// 线程测试
// 	LOG_DEBUG("start thread test\n");
// 	test_thread * tt[10];
// 	for (int i=0; i<10; ++i)
// 	{
// 		tt[i] = gMemory.New<test_thread>();
// 		tt[i]->set_id(i);
// 		tt[i]->start();
// 	}
// 	while (true)
// 	{
// 		bool is_any_live = false;
// 		for (int i=0; i<10; ++i)
// 		{
// 			if (tt[i])
// 			{
// 				if (tt[i]->is_end)
// 				{
// 					gMemory.Delete(tt[i]);
// 					tt[i] = NULL;
// 				}
// 				else
// 				{
// 					is_any_live = true;
// 				}
// 			}
// 		}
// 		if (!is_any_live)
// 		{
// 			break;
// 		}
// 	}
// 	LOG_DEBUG("input any key to go next\n");
// 	getchar();

	// 协程测试
// 	LOG_DEBUG("start coroutine test\n");
// 	CoroutineGroup cg;
// 	for (int i=0; i<10; ++i)
// 	{
// 		test_coroutine * c = gMemory.New<test_coroutine>();
// 		c->id = i + 1;
// 		cg.Add(c);
// 	}
// 	while (cg.GetCoroutineCount() > 0)
// 	{
// 		cg.Update();
// 	}
// 
// 	LOG_DEBUG("input any key to go next\n");
// 	getchar();

	// 网络测试
	LOG_DEBUG("net listen test\n");
	//gMemory.New<listen_thread>()->start();
	gMemory.New<poller_thread>()->start();

	LOG_DEBUG("input any key to end game\n");
	getchar();
    return 0;
}
