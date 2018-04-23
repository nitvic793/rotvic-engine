#include "AsyncLoader.h"
#include <fstream>


void AsyncLoader::AsyncLoadFile(std::string filename, std::function<void(void*)> callback)
{
	auto asyncJob = [callback](void*) {
		std::ifstream file("input.txt", std::ios::binary);
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

void AsyncLoader::CleanUp()
{

}

AsyncLoader::AsyncLoader()
{
}


AsyncLoader::~AsyncLoader()
{
	CleanUp();
}
