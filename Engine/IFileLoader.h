#pragma once

#include <string>

/// <summary>
/// IFileLoader defines the file loader interface
/// </summary>
template<typename T>
class IFileLoader
{
public:
	virtual T Load(std::string fileName) = 0;
};

