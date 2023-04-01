#include "threadpool.h"
#include <iostream>
const int TASK_MAX_THRESHHOLD = INT32_MAX;
const int THREAD_MAX_THRESHHOLD = 1024;
const int THREAD_MAX_IDLE_TIME = 60;

ThreadPool::ThreadPool()
    : initThreadSize_(0)
    , taskSize_(0)
    , idleThreadSzie_(0)
    , taskQueMaxThreshHold_(TASK_MAX_THRESHHOLD)
    , threadSizeThreshHold_(THREAD_MAX_THRESHHOLD)
    , poolMode_(PoolMode::MODE_FIXED)
    , isPoolRunning_(false)
{
}
ThreadPool::~ThreadPool()
{
}

void ThreadPool::setMode(PoolMode mode)
{
    if (checkRunningState())
        return;
    poolMode_ = mode;
}
void ThreadPool::setTaskQueMaxThreshHold(int threshhold)
{
    if (checkRunningState())
        return;
    taskQueMaxThreshHold_ = threshhold;
}
void ThreadPool::setThreadSizeThreshHold(int threshhold)
{
    if (checkRunningState())
        return;
    if (poolMode_ == PoolMode::MODE_CACHED) {
        threadSizeThreshHold_ = threshhold;
    }
}
Result ThreadPool::submitTask(std::shared_ptr<Task> sp)
{
}
void ThreadPool::start(int initThreadSize)
{
    isPoolRunning_ = true;
    initThreadSize = initThreadSize;
    curThreadSize_ = initThreadSize;
    for (int i = 0; i < initThreadSize_; i++) {
        threads_.emplace_back(new Thread(std::bind(&ThreadPool::threadFunc, this, std::placeholders::_1)));
    }
}

void ThreadPool::threadFunc(int threadid)
{
    std::cout << "Begin threadFunc" << std::endl;
    std::cout << std::this_thread::get_id() << std::endl;
    std::cout << "End threadFunc" << std::endl;
}
bool ThreadPool::checkRunningState() const
{
    return isPoolRunning_;
}

///------------------线程方法实现--------------------------/////

Thread::Thread(ThreadFunc func)
    : func_(func)
    , threadId_(generateId_++)
{
}
Thread::~Thread()
{
}
void Thread::start()
{
    std::thread t(func_, threadId_);
    t.detach();
}
int Thread::getId() const
{
    return threadId_;
}
///------------------Task方法--------------------------/////
Task::Task()
    : result_(nullptr)
{
}

void Task::exec()
{
}

void Task::setResult(Result* res)
{
    result_ = res;
}