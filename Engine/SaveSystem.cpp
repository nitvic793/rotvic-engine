/// <summary>
/// Author: Nitish Victor
/// </summary>

#include "SaveSystem.h"

/// <summary>
/// Saves data from serializable class to given file. 
/// </summary>
/// <param name="data">Serializable object</param>
/// <param name="file">Save file name</param>
void SaveSystem::Save(ISerializable *data, std::string file)
{
	std::stringstream strBuffer;
	data->Serialize(strBuffer);
	buffer->Save(file, strBuffer);
}

/// <summary>
/// Loads data from given file and deserializes it. 
/// </summary>
/// <param name="data">Object to deserialize to.</param>
/// <param name="file">Load file name</param>
void SaveSystem::Load(ISerializable *data, std::string file)
{
	data->Deserialize(buffer->Load(file));
}


SaveSystem::SaveSystem()
{
	buffer = new FileBuffer();
}

SaveSystem::SaveSystem(FileBuffer *filebuffer)
{
	if (filebuffer == nullptr) {
		throw std::invalid_argument("Null buffer");
	}
	buffer = filebuffer;
}

SaveSystem::~SaveSystem()
{
	if(buffer)delete buffer;
	buffer = nullptr;
}

