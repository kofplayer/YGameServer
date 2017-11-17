YGAME_SERVER_BEGIN

class EventHandler
{
public:
    virtual void OnEvent(void * pParam, const EventFilter & filter) = 0;
};

YGAME_SERVER_END
