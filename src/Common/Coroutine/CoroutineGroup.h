YGAME_SERVER_BEGIN

class CoroutineGroup
{
public:
    CoroutineGroup();
	virtual ~CoroutineGroup();
	virtual void Add(Coroutine * c);
	virtual bool Remove(Coroutine * c);
	virtual void Update();
	virtual uint32 GetCoroutineCount();
protected:
	YVector<Coroutine*> m_items;
};

YGAME_SERVER_END