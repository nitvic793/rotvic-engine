/// <summary>
/// Author: Nitish Victor
/// </summary>
/// <remarks>
/// Custom implementation of code referred from Juan Palacios'(juan.palacios.puyana@gmail.com).
/// https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
/// </remarks>

#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

/// <summary>
/// Thread-safe queue for use with worker threads. Wraps around std::queue and makes it thread-safe. 
/// </summary>
template<typename T>
class ConcurrentQueue
{
	std::queue<T> mQueue;
	std::mutex mutex;
	std::condition_variable condVar;
public:
	/// <summary>
	/// Pop item from the front of the queue. 
	/// </summary>
	/// <returns>Value of type T</returns>
	T Pop();

	/// <summary>
	/// Enqueue value to back of the queue
	/// </summary>
	/// <param name="item">Item to enqueue</param>
	void Push(const T& item);

	/// <summary>
	/// Returns true if queue is empty
	/// </summary>
	/// <returns>If queue is empty or not</returns>
	bool IsEmpty();

	ConcurrentQueue();
	~ConcurrentQueue();
};


//Have to define template functions in header file to avoid unresolved external symbols.


/// <summary>
/// Pop item from the front of the queue. 
/// </summary>
/// <returns>Value of type T</returns>
template<typename T>
T ConcurrentQueue<T>::Pop()
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

/// <summary>
/// Enqueue value to back of the queue
/// </summary>
/// <param name="item">Item to enqueue</param>
template<typename T>
void ConcurrentQueue<T>::Push(const T & item)
{
	std::unique_lock<std::mutex> mlock(mutex);
	mQueue.push(item);
	mlock.unlock();
	condVar.notify_one();
}

template<typename T>
bool ConcurrentQueue<T>::IsEmpty() 
{
	std::unique_lock<std::mutex> mlock(mutex);
	return mQueue.empty();
}

template<typename T>
ConcurrentQueue<T>::ConcurrentQueue()
{
}

template<typename T>
ConcurrentQueue<T>::~ConcurrentQueue()
{
}
