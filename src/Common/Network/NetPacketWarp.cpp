#include "../Common.h"

YGAME_SERVER_BEGIN

NetPacketWarp::NetPacketWarp()
{
}

NetPacketWarp::~NetPacketWarp()
{
}

bool NetPacketWarp::OnNetRead(SOCKET_ID s)
{
	auto listenerItor = m_netListeners.find(s);
	if (listenerItor != m_netListeners.end())
	{
		NetPConnect * connect = listenerItor->second->Accept();
		if (connect)
		{
			SOCKET_ID newSocket = connect->GetSocket();
			auto itor = m_netInfos.find(newSocket);
			if (itor != m_netInfos.end())
			{
				LOG_ERROR("socket:%d is already in list!!!\n", newSocket);
				assert(false);
				gMemory.Delete(itor->second);
			}
			NetInfo * newInfo = gMemory.New<NetInfo>();
			m_netInfos[newSocket] = newInfo;
			newInfo->connect = connect;
			LOG_DEBUG("Accept %d succ!!!!!!!\n", newSocket);
			m_net_poller.AddRead(newSocket, this);
			m_net_poller.AddWrite(newSocket, this);
			OnNetOpen(connect, s);
			return true;
		}
		else
		{
			LOG_ERROR("Accept %d error!!!\n", s);
			assert(false);
			return false;
		}
	}
	else
	{
		auto itor = m_netInfos.find(s);
		if (itor == m_netInfos.end())
		{
			LOG_ERROR("can not find socket:%d in list!!!\n", s);
			assert(false);
			return false;
		}
		NetInfo * netInfo = (itor->second);
		if (!netInfo->connect->DoReceivePacket())
		{
			OnNetClose(s);
			return false;
		}
		NetPacket * packet = NULL;
		while (packet = netInfo->connect->ReceivePacket())
		{
			OnNetPacket(netInfo->connect->GetSocket(), packet);
			gMemory.Delete(packet);
		}
	}
	return true;
}

bool NetPacketWarp::OnNetWrite(SOCKET_ID s)
{
	auto itor = m_netConnecters.find(s);
	if (itor != m_netConnecters.end())
	{
		NetAConnect * connect = itor->second;
		m_netConnecters.erase(itor);
		auto itor2 = m_netInfos.find(s);
		if (itor2 != m_netInfos.end())
		{
			LOG_ERROR("socket:%d is already in list!!!\n", s);
			assert(false);
			gMemory.Delete(itor2->second);
		}
		NetInfo * newInfo = gMemory.New<NetInfo>();
		m_netInfos[s] = newInfo;
		newInfo->connect = connect;
		LOG_DEBUG("Accept %d succ!!!!!!!\n", s);
		m_net_poller.AddRead(s, this);
		OnNetOpen(connect, INVALID_SOCKET);
		return true;
	}
	else
	{
		DoSendPacket(s);
	}
	return true;
}

bool NetPacketWarp::Listen(NET_ADDR addr, NET_PORT port, NetStatusHandler * handler)
{
	NetListener * netListener = gMemory.New<NetListener>();
	netListener->SetAddr(addr);
	netListener->SetPort(port);
	if (!netListener->Listen())
	{
		gMemory.Delete(netListener);
		LOG_ERROR("listen addr:%u port:%u error", addr, port);
		return false;
	}
	SOCKET_ID s = netListener->GetSocket();
	auto itor = m_netListeners.find(s);
	if (itor != m_netListeners.end())
	{
		LOG_ERROR("listen socket:%d is already in list!!!\n", s);
		gMemory.Delete(itor->second);
		assert(false);
	}
	m_netListeners[s] = netListener;
	m_netStatusHandlers[s] = handler;
	m_net_poller.AddRead(s, this);
	return true;
}

bool NetPacketWarp::Connect(NET_ADDR addr, NET_PORT port, NetStatusHandler * handler)
{
	NetAConnect * netConnecter = gMemory.New<NetAConnect>();
	netConnecter->SetAddr(addr);
	netConnecter->SetPort(port);
	if (!netConnecter->Connect(false))
	{
		gMemory.Delete(netConnecter);
		LOG_ERROR("connect addr:%u port:%u error", addr, port);
		return false;
	}
	SOCKET_ID s = netConnecter->GetSocket();
	auto itor = m_netConnecters.find(s);
	if (itor != m_netConnecters.end())
	{
		LOG_ERROR("connect socket:%d is already in list!!!\n", s);
		gMemory.Delete(itor->second);
		assert(false);
	}
	m_netConnecters[s] = netConnecter;
	m_netStatusHandlers[s] = handler;
	m_net_poller.AddWrite(s, this);
	return true;
}

int32 NetPacketWarp::WaitEvent(int32 waitUS)
{
	return m_net_poller.WaitEvent(waitUS);
}

void NetPacketWarp::DoSendPacket(SOCKET_ID s)
{
	auto itor = m_netInfos.find(s);
	if (itor == m_netInfos.end())
	{
		LOG_ERROR("can not find socket:%d in m_netInfos\n", s);
		assert(false);
		return;
	}
	NetInfo * netInfo = (itor->second);
	netInfo->connect->DoSendPacket();
}

void NetPacketWarp::OnNetOpen(NetConnect * connect, SOCKET_ID listenSocket)
{
	auto itor = m_netStatusHandlers.find(listenSocket != INVALID_SOCKET ? listenSocket : connect->GetSocket());
	if (itor != m_netStatusHandlers.end() && itor->second)
	{
		SOCKET_ID s = connect->GetSocket();
		m_netStatusHandlers[s] = itor->second;
		m_netInfos[s]->handler = itor->second->OnNetOpen(connect);
	}
}

void NetPacketWarp::OnNetClose(SOCKET_ID s)
{
	auto itor = m_netInfos.find(s);
	if (itor != m_netInfos.end())
	{
		NetInfo * netInfo = itor->second;
		m_net_poller.RemoveRead(s);
		m_net_poller.RemoveWrite(s);
		netInfo->connect->Close();
		gMemory.Delete(netInfo);
		m_netInfos.erase(itor);
	}
	else
	{
		LOG_ERROR("can not find socket:%d in list!!!\n", s);
		assert(false);
	}
	auto itor2 = m_netStatusHandlers.find(s);
	if (itor2 != m_netStatusHandlers.end() && itor2->second)
	{
		itor2->second->OnNetClose(s);
		m_netStatusHandlers.erase(itor2);
	}
}

void NetPacketWarp::OnNetPacket(SOCKET_ID s, NetPacket * packet)
{
	NetInfo * netInfo = m_netInfos[s];
	if (netInfo->handler)
	{
		netInfo->handler->OnNetPacket(s, packet);
	}
}

YGAME_SERVER_END
