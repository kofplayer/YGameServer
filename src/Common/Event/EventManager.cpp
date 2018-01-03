#include "../Common.h"

YGAME_SERVER_BEGIN
EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::AddHandler(EventHandler * pEventHandler, const EventFilter & eventFilter)
{
    m_monitor.AddHandler(0, pEventHandler, eventFilter);
}

void EventManager::RemoveHandler(EventHandler * pEventHandler, const EventFilter & eventFilter)
{
    m_monitor.RemoveHandler(0, pEventHandler, eventFilter);
    RemoveHandlerFromFireList(pEventHandler, &eventFilter);
}

void EventManager::RemoveHandler(EventHandler * pEventHandler)
{
    m_monitor.RemoveHandler(pEventHandler);
    RemoveHandlerFromFireList(pEventHandler, NULL);
}

void EventManager::RemoveHandlerFromFireList(const EventHandler * pEventHandler, const EventFilter * pEventFilter)
{
    for (uint32 i=0; i < m_fireList.size(); ++i)
    {
        if (pEventFilter == NULL || *pEventFilter == *m_fireList[i]->pEventFilter)
        {
            EventMonitorNode::HandlerList & handleList = *m_fireList[i]->pHandleList;
            for (uint32 j=0; j < handleList.size(); ++j) {
                if (pEventHandler == handleList[j]) {
                    handleList[j] = NULL;
                }
            }
        }
    }
}

void EventManager::Fire(const EventFilter & eventFilter, void * pParam)
{
    EventMonitorNode::HandlerList handlerList;
    m_monitor.GatherHandler(0, eventFilter, handlerList);
    FireInfo fireInfo(&handlerList, &eventFilter);
    for (uint32 i=0; i<handlerList.size(); ++i)
    {
        if (handlerList[i])
        {
            handlerList[i]->OnEvent(pParam, eventFilter);
        }
    }
}

void EventManager::PushFire(FireInfo * pFireInfo)
{
    m_fireList.push_back(pFireInfo);
}

void EventManager::PopFire()
{
    m_fireList.pop_back();
}

YGAME_SERVER_END