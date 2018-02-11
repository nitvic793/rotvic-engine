#pragma once

#include "Mesh.h"
#include "IFileLoader.h"
#include <string>

/// <summary>
/// Meshloader class to load custom mesh files. 
/// </summary>
class MeshLoader  : public IFileLoader<Mesh*>
{
	SystemCore* core;
public:
	Mesh *Load(std::string filename);
	Mesh *Load(std::string filename, bool otherColor);
	MeshLoader(SystemCore*);
	~MeshLoader();
};

