/// <summary>
/// Author: Nitish Victor
/// </summary>
/// <remarks>
/// Code inspired by this StackOverflow answer https://stackoverflow.com/a/35829560/4209569 by user Jts(https://stackoverflow.com/users/5866175/jts)
/// </remarks>

#pragma once

#include <thread>
#include <functional>
#include "ConcurrentQueue.h"

const unsigned int DEFAULT_WORKERS = 1;

//Job function type
typedef std::function<void(void*)> JobType;

//Job arguments
typedef void* JobArgs;

/// <summary>
/// Struct to describe a job and its arguments.
/// </summary>
struct Job
{
	/// <summary>
	/// Invokable job function
	/// </summary>
	JobType job;

	/// <summary>
	/// Job arguments
	/// </summary>
	JobArgs args;
};

/// <summary>
/// Encapulates worker threads. This will enqueue jobs and run it on a separate thread.
/// </summary>
class WorkerThread
{
	int mWorkerCount;
	ConcurrentQueue<Job> mJobQueue;
	std::vector<std::thread> workers;
	std::thread thread;
	std::mutex mutex;
	std::condition_variable conditionVar;
	bool isRunning;
	std::function<void()> threadWorker; //Thread function
public:
	/// <summary>
	/// Starts given number of worker thread(s).
	/// </summary>
	/// <param name="workerCount">Number of worker threads to spawn.</param>
	void Start(int workerCount = DEFAULT_WORKERS);

	/// <summary>
	/// Stops all worker thread(s).
	/// </summary>
	void Stop();

	/// <summary>
	/// Enqueue a task/job to the worker thread.
	/// </summary>
	/// <param name="job">Struct with job function and arguments</param>
	void EnqueueJob(Job job);

	/// <summary>
	/// Returns true if the worker thread is running, else returns false.
	/// </summary>
	/// <returns></returns>
	bool IsRunning() const;

	WorkerThread();
	~WorkerThread();
};

