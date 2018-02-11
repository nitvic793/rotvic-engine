#include "SaveState.h"



SaveState::SaveState()
{
}


SaveState::~SaveState()
{
}

/// <summary>
/// Serializes entity to `EntityName X Y Z`
/// </summary>
/// <param name="stream"></param>
void SaveState::Serialize(std::ostream & stream)
{
	stream << EntityName << " "<< Position.x << " " << Position.y << " " << Position.z;
}

/// <summary>
/// Deserializes input stream to construct save state. 
/// </summary>
/// <param name="stream"></param>
void SaveState::Deserialize(std::istream & stream)
{
	std::string x, y, z;
	try 
	{
		stream.seekg(std::ios::beg);
		stream >> this->EntityName;
		stream >> x >> y >> z;
		if (x == "" || y == "" || z == "") {
			throw std::exception();
		}
		
		Position.x = (float)std::atof(x.c_str());
		Position.y = (float)std::atof(y.c_str());
		Position.z = (float)std::atof(z.c_str());
	}
	catch (...)
	{
#ifdef _DEBUG
		printf("\nError in parsing save file. Loading default values");
#endif
		this->EntityName = "MainEntity";
		Position = Vector3f(0.f, 0.f, 0.f);
	}
}
