YGAME_SERVER_BEGIN

template<typename T>
class Singleton
{
public:
	Singleton() {};
	virtual ~Singleton() {};
	static T * getInstance()
	{
		if (m_instatce == NULL)
		{
			m_instatce = new T();
		}
		return m_instatce;
	}
// 	static void releaseInstance()
// 	{
// 		if (m_instatce)
// 		{
// 			delete m_instatce;
// 			m_instatce = NULL;
// 		}
// 	}
private:
	static T * m_instatce;
};

template<typename T> T * Singleton<T>::m_instatce = NULL;

YGAME_SERVER_END
