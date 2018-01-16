//
//  main.cpp
//  YGameServer
//
//  Created by 张锦 on 2017/8/12.
//  Copyright © 2017年 张锦. All rights reserved.
//

#include <iostream>
#include "./Common/Common.h"
#include "./Resource/AccountInfo.pb.h"
#include "./Resource/GoodsInfo.pb.h"
#include "./Protocol/cs_msg.h"

using namespace YGAME_SERVER_NAMESPACE;

#ifdef HAS_EPOLL
typedef NetEpollPoller RealNetPoller;
#else
typedef NetSelectPoller RealNetPoller;
#endif

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
            gEvent->RemoveHandler(pH);
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

class poller_thread : public Thread, public Singleton<poller_thread>
{
public:
	poller_thread() 
	{
		m_net_poller = gMemory.New<RealNetPoller>();
	}
	virtual ~poller_thread() 
	{
		gMemory.Delete(m_net_poller);
	}
	void AddRead(SOCKET_ID s, NetReadHandler * handler)
	{
		m_net_poller->AddRead(s, handler);
	}
	void AddWrite(SOCKET_ID s, NetWriteHandler * handler)
	{
		m_net_poller->AddWrite(s, handler);
	}
	void RemoveRead(SOCKET_ID s)
	{
		m_net_poller->RemoveRead(s);
	}
	void RemoveWrite(SOCKET_ID s)
	{
		m_net_poller->RemoveWrite(s);
	}
protected:
	virtual void run()
	{
		while (true)
		{
			m_net_poller->WaitEvent(1000);
		}
	}
private:
	NetPoller * m_net_poller;
};

class listen_thread : public Thread
{
public:
	virtual ~listen_thread() {};
protected:
	virtual void run()
	{
		NetListener * listener = gMemory.New<NetListener>();
		listener->SetAddr(INADDR_ANY);
		listener->SetPort(9999);
		bool succ = listener->Listen();

		NetPConnect * conn = listener->Accept();
		if (conn)
		{
			LOG_DEBUG("Accept succ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		}
	}
private:
	int aa;
};

class net_rw_handler : public NetReadHandler, public NetWriteHandler
{
public:
	virtual bool OnNetRead(SOCKET_ID s)
	{
		int64 len = m_connect->Recv(m_buffer, sizeof(m_buffer));
		if (len > 0)
		{
			m_buffer[len-1] = 0;
			LOG_DEBUG("socket %d recv %s\n", m_connect->GetSocket(), m_buffer);
		}
		else if (len == 0)
		{
			poller_thread::getInstance()->RemoveRead(m_connect->GetSocket());
			m_connect->Close();
		}
		return true;
	}
	virtual bool OnNetWrite(SOCKET_ID s)
	{
		return true;
	}
	NetConnect * m_connect;
private:
	char m_buffer[512];
};

class listen_handler : public NetReadHandler
{
public:
	virtual bool OnNetRead(SOCKET_ID s)
	{

		NetPConnect * conn = m_listener->Accept();
		if (conn)
		{
			LOG_DEBUG("Accept prot %u succ!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", m_listener->GetPort());
			net_rw_handler * handler = gMemory.New<net_rw_handler>();
			handler->m_connect = conn;
			poller_thread::getInstance()->AddRead(conn->GetSocket(), handler);
			poller_thread::getInstance()->AddWrite(conn->GetSocket(), handler);
		}
		return true;
	}

public:
	NetListener * m_listener;
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

struct GamePacketHead
{
	uint32 len;
	uint32 cmd;
	uint32 GetDataLen()
	{
		return len;
	}
};

class GamePacket : public NetPacket
{
public:
	GamePacketHead * GetHead()
	{
		return ((GamePacketHead*)GetPacket());
	}
	uint32 GetDataLen()
	{
		return GetHead()->len;
	}
	char * GetData()
	{
		return GetPacket() + sizeof(GamePacketHead);
	}
};

GamePacket * PacketGameMsg(uint32 cmd, ::google::protobuf::Message * msg)
{
	int32 size = msg->ByteSize();
	GamePacket * packet = gMemory.New<GamePacket>();
	packet->Init(size+sizeof(GamePacketHead));
	GamePacketHead * head = packet->GetHead();
	head->len = size;
	head->cmd = cmd;
	if (msg->SerializeToArray(packet->GetData(), packet->GetDataLen()))
	{
		return packet;
	}
	else
	{
		gMemory.Delete(packet);
		return NULL;
	}
}

class ServerPlayerMgr;

class ServerPlayer : public NetPacketHandler
{
public:
	ServerPlayer() : m_id(0), m_connect(NULL), m_playerMgr(NULL) {}
	virtual ~ServerPlayer(){}
	virtual void OnNetPacket(SOCKET_ID s, NetPacket * packet)
	{
		LOG_DEBUG("receive player:%u packet len %u\n", m_id, packet->GetPacketLen());
		GamePacket * gamePacket = dynamic_cast<GamePacket*>(packet);
		GamePacketHead * head = gamePacket->GetHead();
		switch (head->cmd)
		{
		case CS_MSG_LOGIN_REQ:
			{
				LoginReq * req = gMemory.New<LoginReq>();
				if (req->ParseFromArray(gamePacket->GetData(), gamePacket->GetDataLen()))
				{
					LOG_DEBUG("player login account:%s password:%s\n", req->account().c_str(), req->password().c_str());
					LoginRsp * rsp = gMemory.New<LoginRsp>();
					rsp->set_result(0);
					rsp->set_userid(m_id);
					rsp->set_nickname(req->account().c_str());
					SendMsg(CS_MSG_LOGIN_RSP, rsp);
					gMemory.Delete(rsp);
				}
				gMemory.Delete(req);
			}
			break;
		case CS_MSG_CHAT_REQ:
			{
				ChatReq * req = gMemory.New<ChatReq>();
				if (req->ParseFromArray(gamePacket->GetData(), gamePacket->GetDataLen()))
				{
					LOG_DEBUG("player chat:%s\n", req->msg().c_str());
					ChatRsp * rsp = gMemory.New<ChatRsp>();
					rsp->set_result(0);
					SendMsg(CS_MSG_CHAT_RSP, rsp);
					gMemory.Delete(rsp);

					ChatNtf * ntf = gMemory.New<ChatNtf>();
					ntf->set_msg(req->msg().c_str());
					SendMsgToAll(CS_MSG_CHAT_NTF, ntf);
					gMemory.Delete(ntf);
				}
				gMemory.Delete(req);
			}
			break;
		default:
			break;
		}
		
	}
	virtual bool SendMsg(uint32 cmd, ::google::protobuf::Message * msg)
	{
		GamePacket * packet = PacketGameMsg(cmd, msg);
		if (msg->SerializeToArray(packet->GetData(), packet->GetDataLen()))
		{
			m_connect->SendPacket(packet);
			return true;
		}
		else
		{
			gMemory.Delete(packet);
			return false;
		}
	}
	virtual bool SendMsgToAll(uint32 cmd, ::google::protobuf::Message * msg);
	uint32 m_id;
	NetConnect * m_connect;
	ServerPlayerMgr * m_playerMgr;
};

class ServerPlayerMgr : public NetStatusHandler
{
public:
	ServerPlayerMgr() : m_basePlayerID(0) {}
	virtual ~ServerPlayerMgr(){}
	virtual NetPacketHandler * OnNetOpen(NetConnect * connect)
	{
		ServerPlayer * player = gMemory.New<ServerPlayer>();
		player->m_id = ++m_basePlayerID;
		player->m_connect = connect;
		player->m_playerMgr = this;
		connect->SetNetPacketer<HeadNetPacketer<GamePacket, GamePacketHead>/**/>();
		m_players[connect->GetSocket()] = player;
		LOG_DEBUG("player %u connect\n", player->m_id);
		return player;
	}
	virtual void OnNetClose(SOCKET_ID s)
	{
		LOG_DEBUG("player %u disconnect\n", m_players[s]->m_id);
		gMemory.Delete(m_players[s]);
		m_players.erase(s);
	}
	virtual bool SendMsgToAll(uint32 cmd, ::google::protobuf::Message * msg)
	{
		GamePacket * packet = PacketGameMsg(cmd, msg);
		if (msg->SerializeToArray(packet->GetData(), packet->GetDataLen()))
		{
			gMemory.IncRefCount(packet, m_players.size());
			for (auto itor=m_players.begin(); itor!=m_players.end(); ++itor)
			{
				itor->second->m_connect->SendPacket(packet);
			}
			gMemory.Delete(packet);
			return true;
		}
		else
		{
			gMemory.Delete(packet);
			return false;
		}
	}
	YMap<SOCKET_ID, ServerPlayer*> m_players;
	uint32 m_basePlayerID;
};

bool ServerPlayer::SendMsgToAll(uint32 cmd, ::google::protobuf::Message * msg)
{
	return m_playerMgr->SendMsgToAll(cmd, msg);
}

class ClientPlayer : public NetPacketHandler, public NetStatusHandler
{
public:
	ClientPlayer() : m_id(0), m_connect(NULL){}
	virtual ~ClientPlayer(){}
	virtual NetPacketHandler * OnNetOpen(NetConnect * connect)
	{
		m_connect = connect;
		connect->SetNetPacketer<HeadNetPacketer<GamePacket, GamePacketHead>/**/>();
		LOG_DEBUG("client connect\n");
		LoginReq * req = gMemory.New<LoginReq>();
		req->set_account("acount11111");
		req->set_password("pwd11111");
		SendMsg(CS_MSG_LOGIN_REQ, req);
		gMemory.Delete(req);
		return this;
	}
	virtual void OnNetClose(SOCKET_ID s)
	{
		LOG_DEBUG("client %u disconnect\n");
		m_connect = NULL;
	}
	virtual void OnNetPacket(SOCKET_ID s, NetPacket * packet)
	{
		LOG_DEBUG("receive player:%u packet len %u\n", m_id, packet->GetPacketLen());
		GamePacket * gamePacket = dynamic_cast<GamePacket*>(packet);
		GamePacketHead * head = gamePacket->GetHead();
		switch (head->cmd)
		{
		case CS_MSG_LOGIN_RSP:
			{
				LoginRsp * rsp = gMemory.New<LoginRsp>();
				if (rsp->ParseFromArray(gamePacket->GetData(), gamePacket->GetDataLen()))
				{
					LOG_DEBUG("player login result:%d userid:%u nickname:%s\n", rsp->result(), rsp->userid(), rsp->nickname().c_str());

					ChatReq * req = gMemory.New<ChatReq>();
					req->set_msg("hello every one!!!\n");
					SendMsg(CS_MSG_CHAT_REQ, req);
					gMemory.Delete(req);
				}
				gMemory.Delete(rsp);
			}
			break;
		case CS_MSG_CHAT_RSP:
			{
				ChatRsp * rsp = gMemory.New<ChatRsp>();
				if (rsp->ParseFromArray(gamePacket->GetData(), gamePacket->GetDataLen()))
				{
					LOG_DEBUG("player chat result:%d\n", rsp->result());
				}
				gMemory.Delete(rsp);
			}
			break;
		case CS_MSG_CHAT_NTF:
			{
				ChatNtf * ntf = gMemory.New<ChatNtf>();
				if (ntf->ParseFromArray(gamePacket->GetData(), gamePacket->GetDataLen()))
				{
					LOG_DEBUG("someone chat:%s\n", ntf->msg().c_str());
				}
				gMemory.Delete(ntf);
			}
			break;
		default:
			break;
		}

	}
	virtual bool SendMsg(uint32 cmd, ::google::protobuf::Message * msg)
	{
		GamePacket * packet = PacketGameMsg(cmd, msg);
		if (msg->SerializeToArray(packet->GetData(), packet->GetDataLen()))
		{
			LOG_DEBUG("sendmsg cmd:%u, len:%u ok\n", cmd, packet->GetDataLen());
			m_connect->SendPacket(packet);
			return true;
		}
		else
		{
			LOG_ERROR("sendmsg cmd:%u serialize faile\n", cmd);
			gMemory.Delete(packet);
			return false;
		}
	}
	uint32 m_id;
	NetConnect * m_connect;
};

int main(int argc, const char * argv[]) {
    SET_LOG_LEVEL(LL_DEBUG);
    LOG_ADD_WRITER(new ScreenLogWriter());
    LOG_ADD_WRITER(new FileLogWriter());
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
//     gEvent->AddHandler(pH1, EventFilter(1));
//     gEvent->AddHandler(pH2, EventFilter(1,2,3));
//     gEvent->AddHandler(pH3, EventFilter(1,2,3));
//     gEvent->Fire(EventFilter(1,2,3), NULL);
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

	NET_PORT port = 7777;
	NET_ADDR addr = 115 << 24 | 159 << 16 | 33 << 8 | 215;

	NetPacketWarp playerNet;

	ServerPlayerMgr playerMgr;
	playerNet.Listen(INADDR_ANY, port, &playerMgr);

	/*
	ClientPlayer player1;
	ClientPlayer player2;
	ClientPlayer player3;
	playerNet.Connect(addr, port, &player1);
	playerNet.Connect(addr, port, &player2);
	playerNet.Connect(addr, port, &player3);
	*/
	/*
	poller_thread::getInstance()->start();

	NetListener * listener = gMemory.New<NetListener>();
	listener->SetAddr(INADDR_ANY);
	listener->SetPort(8888);
	bool succ = listener->Listen();
	listen_handler * listenHandler = gMemory.New<listen_handler>();
	listenHandler->m_listener = listener;
	poller_thread::getInstance()->AddRead(listener->GetSocket(), listenHandler);

	listener = gMemory.New<NetListener>();
	listener->SetAddr(INADDR_ANY);
	listener->SetPort(9999);
	succ = listener->Listen();
	listenHandler = gMemory.New<listen_handler>();
	listenHandler->m_listener = listener;
	poller_thread::getInstance()->AddRead(listener->GetSocket(), listenHandler);
	*/

	// 读取资源
	FILE * fd = fopen("GoodsInfo.bin", "r");
	if (fd) {
		fseek(fd, 0, SEEK_END);
		
		fpos_t pos_len, pos_pos;
		memset(&pos_pos, 0, sizeof(pos_pos));
		fgetpos(fd, &pos_len);
		fsetpos(fd, &pos_pos);
#if CURRENT_PLATFORM == PLATFORM_WIN32
		uint32 len = pos_len;
#else
		uint32 len = pos_len.__pos;
#endif
		void * p = gMemory.Malloc(len);
		fread(p, 1, len, fd);
		fclose(fd);
		X::Res::GoodsInfo_ARRAY infos;
		infos.ParseFromArray(p, len);
		int count = infos.items_size();
		for (int i=0; i<count; ++i)
		{
			const ::X::Res::GoodsInfo& info = infos.items(i);
			LOG_DEBUG("res:%u\n", info.goods_id());
		}
		gMemory.Free(p);
	}


	//db测试
 	DataBase * db = gMemory.New<DataBaseMySql>();
 	db->SetInfo("116.62.50.103", 3306, "root", "4391007", "king_flower");
 	DBResult * dbResult = gMemory.New<DBResultMySql>();
 	db->Query("select * from user_info where user_type=0", dbResult);
 	uint32 f = dbResult->GetFieldCount();
 	uint32 r = dbResult->GetRowCount();
 	while (!dbResult->IsEnd())
 	{
 		std::string nick_name = dbResult->GetFieldValue("nick_name");
 		std::string head_url = dbResult->GetFieldValue("head_url");
 		LOG_DEBUG("head_url %s\n", head_url.c_str());
 		dbResult->Next();
 	}
	LOG_DEBUG("game start ok!!!\n");

	while (true)
	{
		playerNet.WaitEvent(100000);
	}

    return 0;
}
