/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "AsyncLoader.h"
#include <fstream>

void test(void* args)
{

}

void AsyncLoader::Loader(void * args)
{
}

void AsyncLoader::AsyncLoadFile(std::string filename, std::function<void(void*, unsigned int)> callback)
{
	auto asyncJob = [filename, callback](void* args) {
		std::ifstream file(filename.c_str() , std::ios::binary);
		file.seekg(0, std::ios::end);
		unsigned int size = (unsigned int)file.tellg();
		file.seekg(0, std::ios::beg);
		char *data = new char[size];
		file.read(data, size);
		file.close();
		callback(data, size);
	};

	asyncWorker->EnqueueJob(
	Job{
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
