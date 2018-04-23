/// <summary>
/// Author: Nitish Victor
/// </summary>

#pragma once

#include <thread>
#include <functional>
#include "ConcurrentQueue.h"

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
/// Encapulates one worker thread. This will enqueue jobs and run it on a separate thread.
/// </summary>
class WorkerThread
{
	ConcurrentQueue<Job> mJobQueue;
	std::thread thread;
	std::mutex mutex;
	std::condition_variable conditionVar;
	bool isRunning;
	std::function<void()> threadWorker; //Thread function
public:
	/// <summary>
	/// Starts the worker thread.
	/// </summary>
	void Start();

	/// <summary>
	/// Stops the worker thread.
	/// </summary>
	void Stop();

	/// <summary>
	/// Enqueue a task/job to the worker thread.
	/// </summary>
	/// <param name="job"></param>
	void EnqueueJob(Job job);

	/// <summary>
	/// Returns true if the worker thread is running, else returns false.
	/// </summary>
	/// <returns></returns>
	bool IsRunning() const;

	WorkerThread();
	~WorkerThread();
};

