#pragma once
#include <queue>
#include <thread>
#include <future>

class AsyncLoader
{
	std::queue<std::shared_future<void>> futures;
public:
	void AsyncLoadFile(std::string filename, std::function<void(void*)> callback);
	void CleanUp();
	AsyncLoader();
	~AsyncLoader();
};

