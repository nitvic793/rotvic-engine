#pragma once
#include <queue>
#include <thread>
#include <future>
#include "WorkerThread.h"

class AsyncLoader
{
	WorkerThread *asyncWorker;
public:
	void Loader(void* args);
	void AsyncLoadFile(std::string filename, std::function<void(void*)> callback);
	AsyncLoader(WorkerThread* worker);
	~AsyncLoader();
};

