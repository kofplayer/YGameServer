YGAME_SERVER_BEGIN
class Task;
typedef void (*TaskCallBackFunc)(bool isSucc, Task * pTask);

class Task
{
public:
    Task();
    virtual ~Task();
    void start();
    bool addTask(Task * pTask);
    void setCallBackFunc(TaskCallBackFunc pFunc);
protected:
    virtual void onStart();
    virtual void onEnd(bool isSucc);
    void doEnd(bool isSucc, Task * pTask = NULL);
private:
    void doStart(Task * pTask);
    void end(bool isSucc);
    void onAllEnd(bool isSucc, Task * pTask);
    void nextTask();
    Task * getCurTask();
    void startNextTask();
    YVector<Task*> m_childs;
    Task * m_pParent;
    TaskCallBackFunc m_pCallBackFunc;
    int32 m_curTaskIndex;
};
YGAME_SERVER_END