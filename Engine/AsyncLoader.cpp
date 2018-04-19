#include "AsyncLoader.h"
#include <fstream>


void AsyncLoader::AsyncLoadFile(std::string filename, std::function<void(void*)> callback)
{
	auto fut = std::async(std::launch::async, [callback]() {
		std::ifstream file("input.txt", std::ios::binary);
		file.seekg(0, std::ios::end);
		int size = file.tellg();
		file.seekg(0, std::ios::beg);
		char *data = new char[size];
		file.read(data, size);
		file.close();
		callback(data);
	});
	futures.push(move(fut));
}

void AsyncLoader::CleanUp()
{
	int i = 0;
	while (!futures.empty())
	{
		auto f = futures.front();
		if (f._Is_ready())futures.pop();
		else break;
	}
}

AsyncLoader::AsyncLoader()
{
}


AsyncLoader::~AsyncLoader()
{
	CleanUp();
}
