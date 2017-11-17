YGAME_SERVER_BEGIN
class EventManager : public Singleton<EventManager>
{
public:
	EventManager();
	virtual ~EventManager();
    void addHandler(EventHandler * pEventHandler, const EventFilter & eventFilter);
    void removeHandler(EventHandler * pEventHandler, const EventFilter & eventFilter);
    void removeHandler(EventHandler * pEventHandler);
    void fire(const EventFilter eventFilter, void * pParam);
private:
    class FireInfo
    {
    public:
        EventMonitorNode::HandlerList * pHandleList;
        const EventFilter * pEventFilter;
        FireInfo(EventMonitorNode::HandlerList * pHandleList, const EventFilter * pEventFilter)
        {
            EventManager::getInstance()->pushFire(this);
            this->pHandleList = pHandleList;
            this->pEventFilter = pEventFilter;
        }
        ~FireInfo()
        {
            EventManager::getInstance()->popFire();
        }
    };
    void removeHandlerFromFireList(const EventHandler * pEventHandler, const EventFilter * pEventFilter);
    void pushFire(FireInfo * pFireInfo);
    void popFire();
    typedef Vector<FireInfo*> FireList;
    FireList m_fireList;
    EventMonitorNode m_monitor;
};

#define gEvent EventManager::getInstance()
YGAME_SERVER_END
