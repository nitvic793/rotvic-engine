#include "ConfigLoader.h"
#include <fstream>


ConfigLoader::ConfigLoader()
{
}


ConfigLoader::~ConfigLoader()
{
}

/// <summary>
/// Loads configuration data from JSON file. 
/// </summary>
/// <param name="file">Config file name</param>
/// <returns>Key-Value pair map of configuration</returns>
/// <remarks>Does not support nested JSON yet.</remarks>
ConfigMap ConfigLoader::Load(std::string file)
{
	Json::Value root;
	std::ifstream config_doc(file, std::ifstream::binary);
	config_doc >> root;
	auto memberNames = root.getMemberNames();
	for (auto member : memberNames)
	{
		configMap.insert(ConfigMap::value_type(member, root.get(member,"").asString()));
	}
	config_doc.close();
	return configMap;
}
