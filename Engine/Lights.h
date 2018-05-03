#pragma once

#include <DirectXMath.h>
using namespace DirectX;

/// <summary>
/// The light type enum
/// </summary>
enum LightType { Directional, Point };

/// <summary>
/// Base class for light. 
/// </summary>
class ILight {};

/// <summary>
/// Directional light. 
/// </summary>
struct DirectionalLight : public ILight
{
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

/// <summary>
/// Point light.
/// </summary>
struct PointLight : public ILight
{
	XMFLOAT4 Color;
	XMFLOAT3 Position;
};


/// <summary>
/// Defines a light. Can contain any of the light types. 
/// </summary>
struct Light
{
	/// <summary>
	/// Instance of light.
	/// </summary>
	ILight *LightInstance;

	/// <summary>
	/// Type of light.
	/// </summary>
	LightType Type;

	/// <summary>
	/// Get the light in its child class type. 
	/// </summary>
	template<typename T>
	T* GetLight()
	{
		return static_cast<T*>(LightInstance);
	}
};

/// <summary>
/// Lights map dictionary.
/// </summary>
typedef std::unordered_map<std::string, Light*> LightsMap;