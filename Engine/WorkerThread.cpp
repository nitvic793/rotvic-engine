/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "WorkerThread.h"

void WorkerThread::Start(int workerCount)
{
	mWorkerCount = workerCount;
	//Do not let running condition or thread to be set when it is already running
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (isRunning) return;
		isRunning = true;
	}
	//Start worker threads
	for (int i = 0; i < workerCount; ++i)
	{
		workers.push_back(std::thread(threadWorker));
	}
}

void WorkerThread::Stop()
{
	std::lock_guard<std::mutex> lock(mutex);
	if (!isRunning)return;
	isRunning = false;
	conditionVar.notify_all(); //Notify thread 
}

void WorkerThread::EnqueueJob(Job job)
{
	mJobQueue.Push(job);
	conditionVar.notify_all(); //Notify thread if there is a new job.
}

bool WorkerThread::IsRunning() const
{
	return isRunning;
}

WorkerThread::WorkerThread()
{
	mWorkerCount = DEFAULT_WORKERS;
	isRunning = false;
	threadWorker = [&]
	{
		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(mutex);
				if (!isRunning)
				{
					break;
				}
				conditionVar.wait(lock, [&] {return !isRunning + !mJobQueue.IsEmpty(); });
			}

			while (!mJobQueue.IsEmpty())
			{
				auto task = mJobQueue.Pop();
				if (task.job) //Check if job is valid
				{
					task.job(task.args); //Invoke job
				}
			}
		}
	};
}


WorkerThread::~WorkerThread()
{
	Stop();
	for (auto &worker : workers)
	{
		worker.join();
	}
	//thread.join();
}
