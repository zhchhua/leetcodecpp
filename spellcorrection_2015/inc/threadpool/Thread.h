#ifndef THREAD_H_
#define THREAD_H_

#include <functional>
#include <pthread.h>
#include "NonCopyable.h"

    
class Thread : private NonCopyable 
{
public:
    typedef std::function<void ()> ThreadCallback;

    explicit Thread(ThreadCallback callback);
    ~Thread();

    void start();
    void join();

private:
    static void *runInThread(void *);

    pthread_t threadId_;
    bool isRunning_;
    ThreadCallback callback_; //回调函数
};

#endif //THREAD_H_
