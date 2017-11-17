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
    std::cout << "onTaskCallEnd " << isSucc << "\n";
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
        std::cout << "onStart " << m_index << "\n";
        doEnd(m_isSucc);
    }
    virtual void onEnd(bool isSucc)
    {
        std::cout << "onEnd " << m_index << " " << isSucc << "\n";
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
        std::cout << m_n << "\n";
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
	void set_id(int id)
	{
		aa = id;
	}
	virtual ~test_thread() {};
protected:
	virtual void run()
	{
		for (int i = 0; i < 10; ++i)
		{
			std::cout << aa << "--" << i << " Hello, World!\n";
			Sleep(1000);
		}
		gMemory.Delete(this);
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
			std::cout << "Accept succ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		}
	}
private:
	int aa;
};

int main(int argc, const char * argv[]) {

	// 事件测试
    test_handler * pH1 = gMemory.New<test_handler>();
	pH1->set_handler_info(1);
	test_handler * pH2 = gMemory.New<test_handler>();
	pH2->set_handler_info(2);
	test_handler * pH3 = gMemory.New<test_handler>();
	pH3->set_handler_info(3);
    
    pH1->pH = pH2;
    pH2->pH = pH3;
    
    gEvent->addHandler(pH1, EventFilter(1));
    gEvent->addHandler(pH2, EventFilter(1,2,3));
    gEvent->addHandler(pH3, EventFilter(1,2,3));
    gEvent->fire(EventFilter(1,2,3), NULL);

	gMemory.Delete(pH1);
	gMemory.Delete(pH2);
	gMemory.Delete(pH3);

	// 事务测试
	gMemory.Init<testTask>(100, 10);
    
    testTask * task = gMemory.New<testTask>();
    task->setCallBackFunc(onTaskCallEnd);
    
    testTask * task1 = gMemory.New<testTask>(); 
	task1->set_task_info(1, true);
    task->addTask(task1);
    
	testTask * task2 = gMemory.New<testTask>();
	task2->set_task_info(2, true);
    task1->addTask(task2);
    
	testTask * task3 = gMemory.New<testTask>();
	task3->set_task_info(3, true);
    task2->addTask(task3);
    
	testTask * task4 = gMemory.New<testTask>();
	task4->set_task_info(4, true);
    task1->addTask(task4);
    
	testTask * task5 = gMemory.New<testTask>();
	task5->set_task_info(5, true);
    task3->addTask(task5);
    
    task->start();
    
	gMemory.Delete(task);
	gMemory.Delete(task1);
	gMemory.Delete(task2);
	gMemory.Delete(task3);
	gMemory.Delete(task4);
	gMemory.Delete(task5);

	// 线程测试
	for (int i=0; i<10; ++i)
	{
		test_thread * pThread = gMemory.New<test_thread>();
		pThread->set_id(i);
		pThread->start();
	}

	// 网络测试
	gMemory.New<listen_thread>()->start();

// 	NetListener * listener = gMemory.New<NetListener>();
// 	bool succ = listener->Create();
// 	succ = listener->Bind(INADDR_ANY, 9999);
// 	succ = listener->Listen();
// 
// 	NetPConnect * conn = listener->Accept();
// 	if (conn)
// 	{
// 		std::cout << "Accept succ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
// 	}
    
    // insert code here...
    std::cout << "Hello, World!\n";

	getchar();
    return 0;
}
