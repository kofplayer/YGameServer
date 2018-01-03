YGAME_SERVER_BEGIN
template<typename T>
class EventMonitorNodeBase
{
public:
    typedef YVector<EventHandler*> HandlerList;
    typedef YMap<T, EventMonitorNodeBase<T>/**/> NodeMap;
    EventMonitorNodeBase() {};
    virtual ~EventMonitorNodeBase() {};
    void GatherHandler(uint32 deep, const EventFilterBase<T> & filter, HandlerList & rHandlerList)
    {
        for (uint32 i=0; i<m_handlerList.size(); ++i)
        {
            bool bExist = false;
            for (uint32 j=0; j<rHandlerList.size(); ++j) {
                if (rHandlerList[j] == m_handlerList[i])
                {
                    bExist = true;
                    break;
                }
            }
            if (!bExist) {
                rHandlerList.push_back(m_handlerList[i]);
            }
        }
        if (deep >= filter.GetFilterCount())
        {
            return;
        }
		auto itor = m_nodeMap.find(filter.GetFilter(deep));
        if (itor != m_nodeMap.end())
        {
            itor->second.GatherHandler(deep+1, filter, rHandlerList);
        }
    }
    void AddHandler(uint32 deep, EventHandler * pHandler, const EventFilterBase<T> & filter)
    {
        if (NULL == pHandler)
        {
            return;
        }
        if (deep >= filter.GetFilterCount())
        {
            if (!IsHandlerExist(pHandler))
            {
                m_handlerList.push_back(pHandler);
            }
            return;
        }
        m_nodeMap[filter.GetFilter(deep)].AddHandler(deep+1, pHandler, filter);
    }
    void RemoveHandler(uint32 deep, EventHandler * pHandler, const EventFilterBase<T> & filter)
    {
        if (NULL == pHandler)
        {
            return;
        }
        if (deep >= filter.GetFilterCount())
        {
            for (uint32 i=0; i<m_handlerList.size(); ++i)
            {
                if (pHandler == m_handlerList[i])
                {
                    m_handlerList.erase(m_handlerList.begin()+i);
                    return;
                }
            }
        }
        m_nodeMap[filter.GetFilter(deep)].RemoveHandler(deep+1, pHandler, filter);
    }
    void RemoveHandler(EventHandler * pHandler)
    {
        for (uint32 i=0; i<m_handlerList.size(); ++i)
        {
            if (pHandler == m_handlerList[i])
            {
                m_handlerList.erase(m_handlerList.begin()+i);
                break;
            }
        }
        for (auto itor = m_nodeMap.begin(); itor != m_nodeMap.end(); ++itor)
        {
            itor->second.RemoveHandler(pHandler);
        }
    }
    bool IsHandlerExist(EventHandler * pHandler)
    {
        for (uint32 i=0; i<m_handlerList.size(); ++i)
        {
            if (pHandler == m_handlerList[i])
            {
                return true;
            }
        }
        return false;
    }
private:
    HandlerList m_handlerList;
    NodeMap m_nodeMap;
};
typedef EventMonitorNodeBase<uint32> EventMonitorNode;
YGAME_SERVER_END