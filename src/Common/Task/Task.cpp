#include "../Common.h"

YGAME_SERVER_BEGIN
Task::Task()
{
    m_pCallBackFunc = NULL;
    m_curTaskIndex = -1;
    m_childs.push_back(this);
    m_pParent = NULL;
}

Task::~Task()
{
    
}

void Task::start()
{
    if (m_curTaskIndex != -1) {
        LOG_ERROR("start task index error %d", m_curTaskIndex);
        return;
    }
    if (m_pParent) {
        m_pParent->doStart(this);
    }
    startNextTask();
}

void Task::end(bool isSucc)
{
    if (m_curTaskIndex < 0) {
        LOG_ERROR("end task index error %d", m_curTaskIndex);
        return;
    }
    m_curTaskIndex = -2;
    if (m_pCallBackFunc) {
        m_pCallBackFunc(isSucc, this);
    }
    if (m_pParent) {
        m_pParent->onAllEnd(isSucc,this);
    }
}

bool Task::addTask(Task * pTask)
{
    if (NULL == pTask || this == pTask) {
        LOG_ERROR("addTask task error");
        return false;
    }
    m_childs.push_back(pTask);
    pTask->m_pParent = this;
    return true;
}

void Task::setCallBackFunc(TaskCallBackFunc pFunc)
{
    m_pCallBackFunc = pFunc;
}

void Task::onStart()
{
    
}

void Task::onEnd(bool isSucc)
{
    
}

void Task::doStart(Task * pTask)
{
    if (pTask != getCurTask()) {
        LOG_ERROR("doStart task error");
    }
}

void Task::doEnd(bool isSucc, Task * pTask)
{
    Task * p = pTask;
    if (NULL == p) {
        p = this;
    }
    if (p != getCurTask()) {
        LOG_ERROR("doEnd task error");
        return;
    }
    p->onEnd(isSucc);
    if (!isSucc) {
        end(false);
    }
    startNextTask();
}

void Task::onAllEnd(bool isSucc, Task * pTask)
{
    if (NULL == pTask) {
        LOG_ERROR("onAllEnd task error");
    }
    if (pTask != getCurTask()) {
        LOG_ERROR("onAllEnd task error");
        return;
    }
    if (!isSucc) {
        end(false);
    }
    startNextTask();
}

void Task::nextTask()
{
    ++m_curTaskIndex;
}

Task * Task::getCurTask()
{
    if (m_curTaskIndex >= (int32)m_childs.size()) {
        return NULL;
    }
    return m_childs[m_curTaskIndex];
}

void Task::startNextTask()
{
    nextTask();
    Task * pTask = getCurTask();
    if (pTask) {
        if (pTask == this) {
            pTask->onStart();
        }
        else {
            pTask->start();
        }
    }
    else {
        end(true);
    }
}
YGAME_SERVER_END