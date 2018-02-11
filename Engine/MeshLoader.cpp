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
	Mesh* mesh = new Mesh(core);
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	std::ifstream inFile(filename);
	char in;
	std::vector<DirectX::XMFLOAT3> vertexPositions;
	std::vector<UINT> indices;
	if (!inFile.is_open()) {
		throw std::invalid_argument("Could not load mesh file");
	}

	while (inFile >> in)
	{
		switch (in)
		{
		case 'v':
			DirectX::XMFLOAT3 pos;
			inFile >> pos.x >> pos.y >> pos.z;
			vertexPositions.push_back(pos);
			break;
		case 'f':
		{
			std::string str;
			std::getline(inFile, str);
			std::istringstream ss(str);
			int num;
			while (ss >> num)
			{
				indices.push_back(num - 1);
			}
			break;
		}
		default:
			throw std::invalid_argument("Invalid mesh file format");
		}
	}

	std::vector<Vertex> vertices;
	int i = 0;
	for (auto pos : vertexPositions)
	{
		Vertex v;
		v.Position = pos;
		v.Color = otherColor ? green : ((i % 2) ? red : blue);
		vertices.push_back(v);
		i++;
	}

	if (vertexPositions.size() < 3) {
		throw std::invalid_argument("Invalid mesh file format");
	}

	for (auto index : indices) {
		if (index > vertexPositions.size() - 1) {
			throw std::invalid_argument("Invalid mesh file format");
		}
	}

	mesh->Initialize(vertices.data(), (UINT)vertices.size(), indices.data(), (UINT)indices.size());
	return mesh;
}
