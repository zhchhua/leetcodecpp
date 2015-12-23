#include "ThreadPool.h"
//#include "MyDict.h"


ThreadPool::ThreadPool(size_t queueSize, size_t threadsNum, MyDict &mydict)
: empty_(mutex_),
  full_(mutex_),
  queueSize_(queueSize),
  threadsNum_(threadsNum),
  isStarted_(false),
  mydict_(mydict)
{

}

void ThreadPool::start()
{
    isStarted_ = true;
    //std::vector<std::unique<Thread> >
    for(size_t ix = 0; ix != threadsNum_; ++ix)
    {
        threads_.push_back(
            std::unique_ptr<Thread>(
					new Thread(
						std::bind(&ThreadPool::runInThread, this)
					)
			)
		);
    }
    for(size_t ix = 0; ix != threadsNum_; ++ix)
    {
        threads_[ix]->start();
    }

}

void ThreadPool::addTask(Task task)
{
    MutexLockGuard lock(mutex_);
    while(queue_.size() >= queueSize_)
        empty_.wait();
    //queue_.push(std::move(task));
    queue_.push(task);
    full_.notify();
}


Task ThreadPool::getTask()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty())
        full_.wait();
    Task task = queue_.front();
    queue_.pop();
    empty_.notify();
    return task;
}

void ThreadPool::runInThread(/*Cache& cache*/)
{
    while(1)
    {
        Task task(getTask());
        task.execute(/*cache*/);
    }
}
