#include "MeshLoader.h"
#include <fstream>
#include <sstream>
#include <DirectXMath.h>

using namespace DirectX;

MeshLoader::MeshLoader(SystemCore* core)
{
	this->core = core;
}


MeshLoader::~MeshLoader()
{
}

/// <summary>
/// Loads mesh from given file. Returns an initialized mesh object. 
/// </summary>
/// <param name="filename">The mesh file name</param>
/// <returns>Initialized mesh</returns>
Mesh* MeshLoader::Load(std::string filename)
{
	return Load(filename, false);
}

/// <summary>
/// Loads mesh from given file. Returns an initialized mesh object. 
/// </summary>
/// <param name="filename">The mesh file name</param>
/// <param name="otherColor"></param>
/// <returns>Initialized mesh</returns>
/// <exception cref="invalid_argument">If mesh file is invalid.</exception>
Mesh * MeshLoader::Load(std::string filename, bool otherColor)
{
	return nullptr;
}
