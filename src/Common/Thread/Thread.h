YGAME_SERVER_BEGIN
class Thread
{
public:
    Thread();
    virtual ~Thread();
#if CURRENT_PLATFORM == PLATFORM_WIN32
	static unsigned __stdcall threadFunc(void *arg);
#else	
	static void* threadFunc(void* arg);
#endif
	void start();
protected:
    virtual void run();
private:
	THREAD_ID tidp_;				// 本线程的ID
};
YGAME_SERVER_END