#include "../Common.h"

YGAME_SERVER_BEGIN
EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

void EventManager::addHandler(EventHandler * pEventHandler, const EventFilter & eventFilter)
{
    m_monitor.addHandler(0, pEventHandler, eventFilter);
}

void EventManager::removeHandler(EventHandler * pEventHandler, const EventFilter & eventFilter)
{
    m_monitor.removeHandler(0, pEventHandler, eventFilter);
    removeHandlerFromFireList(pEventHandler, &eventFilter);
}

void EventManager::removeHandler(EventHandler * pEventHandler)
{
    m_monitor.removeHandler(pEventHandler);
    removeHandlerFromFireList(pEventHandler, NULL);
}

void EventManager::removeHandlerFromFireList(const EventHandler * pEventHandler, const EventFilter * pEventFilter)
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

void EventManager::fire(const EventFilter & eventFilter, void * pParam)
{
    EventMonitorNode::HandlerList handlerList;
    m_monitor.gatherHandler(0, eventFilter, handlerList);
    FireInfo fireInfo(&handlerList, &eventFilter);
    for (uint32 i=0; i<handlerList.size(); ++i)
    {
        if (handlerList[i])
        {
            handlerList[i]->OnEvent(pParam, eventFilter);
        }
    }
}

void EventManager::pushFire(FireInfo * pFireInfo)
{
    m_fireList.push_back(pFireInfo);
}

void EventManager::popFire()
{
    m_fireList.pop_back();
}

YGAME_SERVER_END