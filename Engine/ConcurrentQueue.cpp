#include "ConcurrentQueue.h"

template<typename T>
T ConcurrentQueue<T>::pop()
{
	std::unique_lock<std::mutex> mlock(mutex);
	while (mQueue.empty())
	{
		condVar.wait(mlock);
	}
	auto item = mQueue.front();
	mQueue.pop();
	return item;
}

template<typename T>
void ConcurrentQueue<T>::Push(const T & item)
{
	std::unique_lock<std::mutex> mlock(mutex);
	mQueue.push(item);
	mlock.unlock();
	condVar.notify_one();
}

template<typename T>
ConcurrentQueue<T>::ConcurrentQueue()
{
}

template<typename T>
ConcurrentQueue<T>::~ConcurrentQueue()
{
}
