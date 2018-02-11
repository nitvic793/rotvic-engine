#pragma once

#include <map>
#include "json\json.h"
#include "IFileLoader.h"
#include "Constants.h"

typedef std::map<std::string, std::string> ConfigMap;

/// <summary>
/// Config loader. Loads configuration from JSON file to a map.
/// </summary>
class ConfigLoader : public IFileLoader<ConfigMap>
{
	ConfigMap configMap;
	std::vector<std::string> configKeys;
public:
	ConfigMap Load(std::string file);
	ConfigLoader();
	~ConfigLoader();
};
