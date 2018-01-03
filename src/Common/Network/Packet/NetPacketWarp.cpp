#include "../../Common.h"

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
			OnNetOpen(newSocket, s);
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
		char buff[512];
		bool bFirstRead = true;
		bool need_continue = true;
		while (need_continue)
		{
			int64 len = netInfo->connect->Recv(buff, sizeof(buff));
			char * p = buff;
			if (len == 0)
			{
				// 第一次就读取0表示客户端主动断开连接
				if (bFirstRead)
				{
					OnNetClose(s);
					return false;
				}
				else
				{
					return true;
				}
			}
			// 小于0出错了
			else if (len < 0)
			{
				OnNetClose(s);
				return false;
			}
			LOG_DEBUG("socket:%d receive len %llu\n", netInfo->connect->GetSocket(), len);
			bFirstRead = false;

			// 如果读入出来的长度不小于缓冲区大小表示可能还有数据需要读取
			need_continue = (len >= sizeof(buff));

			while (len > 0) 
			{
				// 如果没有包就创建一个
				if (netInfo->receives.empty())
				{
					NetPacketReceiveInfo * info = gMemory.New<NetPacketReceiveInfo>();
					info->packet = gMemory.New<NetPacket>();
					netInfo->receives.push_back(info);
				}
				NetPacketReceiveInfo * packetInfo = (*(--(netInfo->receives.end())));
				// 如果最后一个包已经接收完成就重新创建一个
				if (packetInfo->IsFinish())
				{
					NetPacketReceiveInfo * info = gMemory.New<NetPacketReceiveInfo>();
					info->packet = gMemory.New<NetPacket>();
					netInfo->receives.push_back(info);
					packetInfo = (*(--(netInfo->receives.end())));
				}
				uint16 doLen = 0;
				// 读取消息长度
				if (len > 0)
				{
					if (packetInfo->didHeadLen < NetPacketHeadLen)
					{
						doLen = NetPacketHeadLen-packetInfo->didHeadLen;
						if (doLen > len)
						{
							doLen = len;
						}
						memcpy(&packetInfo->headData[packetInfo->didHeadLen], p, doLen);
						packetInfo->didHeadLen += doLen;
						len -= doLen;
						p += doLen;

						if (packetInfo->didHeadLen >= NetPacketHeadLen)
						{
							uint16 dataLen = ((NetPacketHead*)packetInfo->headData)->dataLen;
							LOG_DEBUG("socket:%d receive packet head finish, data len %u\n", s, dataLen);
							packetInfo->packet->Init(dataLen);
						}
					}
				}
				//读取消息体
				if (len > 0)
				{
					uint32 dataLen = packetInfo->packet->GetDataLen();
					if (packetInfo->didDataLen < dataLen)
					{
						doLen = dataLen - packetInfo->didDataLen;
						if (doLen > len)
						{
							doLen = len;
						}
						memcpy(&(packetInfo->packet->GetData()[packetInfo->didDataLen]), p, doLen);
						packetInfo->didDataLen += doLen;
						len -= doLen;
						p += doLen;
						if (packetInfo->IsFinish())
						{
							LOG_DEBUG("socket:%d receive packet finish, packet len %u\n", s, packetInfo->packet->GetPacketLen());
						}
					}
				}
			}
		}
		auto itor2 = netInfo->receives.begin();
		while (itor2 != netInfo->receives.end())
		{
			NetPacketReceiveInfo * info = (*itor2);
			if (!info->IsFinish())
			{
				break;
			}
			OnNetPacket(netInfo->connect->GetSocket(), info->packet);
			gMemory.Delete(info);
			itor2 = netInfo->receives.erase(itor2);
		}
	}
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
		OnNetOpen(s, INVALID_SOCKET);
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

void NetPacketWarp::SendPacket(SOCKET_ID s, NetPacket * packet)
{
	auto itor = m_netInfos.find(s);
	if (itor == m_netInfos.end())
	{
		return;
	}
	NetInfo * netInfo = (itor->second);
	NetPacketSendInfo * info = gMemory.New<NetPacketSendInfo>();
	info->packet = packet;
	netInfo->sends.push_back(info);
	if (netInfo->sends.size()==1)
	{
		DoSendPacket(s);
	}
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
	PacketSendInfoList & sends = netInfo->sends;
	NetConnect * connect = netInfo->connect;
	if (NULL == connect)
	{
		LOG_ERROR("socket:%d connect is null!!!\n", s);
		assert(false);
		OnNetClose(s);
		return;
	}
	for (auto itor=sends.begin(); itor!=sends.end(); )
	{
		NetPacketSendInfo * info = (*itor);
		if (info->IsFinish())
		{
			LOG_WARN("socket:%d has packet is finish but still in list\n", s);
			itor = sends.erase(itor);
			continue;
		}
		int64 len = connect->Send(info->packet->GetPacket()+info->didLen, info->packet->GetPacketLen()-info->didLen);
		if (len < 0)
		{
			LOG_ERROR("socket:%d send error %d\n", s, len);
			assert(false);
			OnNetClose(s);
			return;
		}
		info->didLen += len;
		if (info->IsFinish())
		{
			LOG_DEBUG("socket:%d send packet finish(%u/%u)\n", s, info->didLen, info->packet->GetPacketLen());
			gMemory.Delete(info);
			itor = sends.erase(itor);
			continue;
		}
		LOG_DEBUG("socket:%d send packet not finish(%u/%u), wait continue\n", s, info->didLen, info->packet->GetPacketLen());
		break;
	}
}

void NetPacketWarp::OnNetOpen(SOCKET_ID s, SOCKET_ID listenSocket)
{
	auto itor = m_netStatusHandlers.find(listenSocket != INVALID_SOCKET ? listenSocket : s);
	if (itor != m_netStatusHandlers.end() && itor->second)
	{
		m_netStatusHandlers[s] = itor->second;
		m_netInfos[s]->handler = itor->second->OnNetOpen(s, this);
	}
}

void NetPacketWarp::OnNetClose(SOCKET_ID s)
{
	auto itor = m_netInfos.find(s);
	if (itor != m_netInfos.end())
	{
		NetInfo * netInfo = itor->second;
		SOCKET_ID s = netInfo->connect->GetSocket();
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
