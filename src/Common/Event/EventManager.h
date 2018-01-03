YGAME_SERVER_BEGIN
class EventManager : public Singleton<EventManager>
{
public:
	EventManager();
	virtual ~EventManager();
    void AddHandler(EventHandler * pEventHandler, const EventFilter & eventFilter);
    void RemoveHandler(EventHandler * pEventHandler, const EventFilter & eventFilter);
    void RemoveHandler(EventHandler * pEventHandler);
    void Fire(const EventFilter & eventFilter, void * pParam);
private:
    class FireInfo
    {
    public:
        EventMonitorNode::HandlerList * pHandleList;
        const EventFilter * pEventFilter;
        FireInfo(EventMonitorNode::HandlerList * pHandleList, const EventFilter * pEventFilter)
        {
            EventManager::getInstance()->PushFire(this);
            this->pHandleList = pHandleList;
            this->pEventFilter = pEventFilter;
        }
        ~FireInfo()
        {
            EventManager::getInstance()->PopFire();
        }
    };
    void RemoveHandlerFromFireList(const EventHandler * pEventHandler, const EventFilter * pEventFilter);
    void PushFire(FireInfo * pFireInfo);
    void PopFire();
    typedef YVector<FireInfo*> FireList;
    FireList m_fireList;
    EventMonitorNode m_monitor;
};

#define gEvent EventManager::getInstance()
YGAME_SERVER_END
