#ifndef _THREADPOOL_H
#define _THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
enum class PoolMode {
    MODE_FIXED, // 模式1：固定线程的数量
    MODE_CACHED, // 模式2：线程数量可动态增长
};

class Result;
class Task {
public:
    Task();
    ~Task() = default;

public:
    void exec();
    void setResult(Result* res);

private:
    Result* result_;
};

class Result {
};

class Thread {
public:
    using ThreadFunc = std::function<void(int)>;

public:
    Thread(ThreadFunc func);
    ~Thread();

public:
    void start();
    int getId() const;

private:
    ThreadFunc func_;
    static int generateId_;
    int threadId_;
};

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

public:
    void setMode(PoolMode mode);
    void setTaskQueMaxThreshHold(int threshhold);
    void setThreadSizeThreshHold(int threshhold);
    Result submitTask(std::shared_ptr<Task> sp);
    void start(int initThreadSize = std::thread::hardware_concurrency());

private:
    void threadFunc(int threadid);
    bool checkRunningState() const;

private:
    std::vector<std::unique_ptr<Thread>> threads_;
    int initThreadSize_;
    int threadSizeThreshHold_;
    std::atomic_int curThreadSize_;
    std::atomic_int idleThreadSzie_;

    std::queue<std::shared_ptr<Task>> taskQue_;
    std::atomic_int taskSize_;
    int taskQueMaxThreshHold_;

    std::mutex taskQueMtx_;
    std::condition_variable notFull_;
    std::condition_variable notEmpty_;
    std::condition_variable exitCond_;

    PoolMode poolMode_;
    std::atomic_bool isPoolRunning_;
};

#endif