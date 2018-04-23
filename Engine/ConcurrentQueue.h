/// <summary>
/// Author: Nitish Victor
/// </summary>
/// <remarks>
/// Code referred from Juan Palacios'(juan.palacios.puyana@gmail.com) implementation.
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
	T pop();
	void Push(const T& item);

	ConcurrentQueue();
	~ConcurrentQueue();
};

