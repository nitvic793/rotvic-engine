#pragma once
#include "ISerializable.h"
#include "FileBuffer.h"

/// <summary>
/// SaveSystem class. Saves and loads save data from file. 
/// </summary>
class SaveSystem
{
protected:
	FileBuffer* buffer;
public:
	void Save(ISerializable *data, std::string file);
	void Load(ISerializable *data, std::string file);
	SaveSystem();
	SaveSystem(FileBuffer*);
	~SaveSystem();
};

