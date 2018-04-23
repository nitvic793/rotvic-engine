/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "WorkerThread.h"

void WorkerThread::Start()
{
	//Do not let running condition or thread to be set when it is already running
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (isRunning) return;
		isRunning = true;
	}
	//Start thread
	thread = std::thread(threadWorker);
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
	isRunning = false;
	threadWorker = [&]
	{
		while (isRunning)
		{
			std::unique_lock<std::mutex> lock(mutex);
			conditionVar.wait(lock, [&] {return !isRunning + !mJobQueue.IsEmpty(); });
			while (!mJobQueue.IsEmpty())
			{
				auto jobPack = mJobQueue.Pop();
				if (jobPack.job)
				{
					jobPack.job(jobPack.args); //Invoke job
				}
			}
		}
	};
}


WorkerThread::~WorkerThread()
{
	Stop();
	thread.join();
}
