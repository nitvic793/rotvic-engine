#pragma once
#include <fstream>
#include <sstream>

/// <summary>
/// FileBuffer class to load/save data to files
/// </summary>
class FileBuffer
{
	std::stringstream buffer;
public:
	virtual std::stringstream& Load(std::string file);
	virtual void Save(std::string file, std::stringstream &buffer);
	FileBuffer();
	~FileBuffer();
};

