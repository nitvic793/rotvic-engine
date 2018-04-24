#include "AsyncLoader.h"
#include <fstream>

void test(void* args)
{

}

void AsyncLoader::Loader(void * args)
{
}

void AsyncLoader::AsyncLoadFile(std::string filename, std::function<void(void*)> callback)
{
	auto asyncJob = [&](void* args) {
		std::ifstream file(filename.c_str() , std::ios::binary);
		file.seekg(0, std::ios::end);
		int size = file.tellg();
		file.seekg(0, std::ios::beg);
		char *data = new char[size];
		file.read(data, size);
		file.close();
		callback(data);
	};

	asyncWorker->EnqueueJob(
	{
		asyncJob,
		nullptr
	});
}

AsyncLoader::AsyncLoader(WorkerThread* worker)
{
	asyncWorker = worker;
}


AsyncLoader::~AsyncLoader()
{
}
