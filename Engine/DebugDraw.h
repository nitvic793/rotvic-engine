#pragma once

#include <DirectXCollision.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
#include <CommonStates.h>
#include "SystemCore.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "PrimitiveShape.h"
#include <queue>
#include <wrl.h>
#include <map>
#include <queue>

using namespace DirectX;

struct DrawCallPayLoad
{
	PrimitiveShapesType shape;
	void* payload;
	Transform transform;
};

/// <summary>
/// Debug Draw system.
/// </summary>
class DebugDraw
{
	//DirectX Related 
	std::unique_ptr<CommonStates> states;
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> batch;
	ID3D11RasterizerState* wireframeRasterizer;

	//Debug Draw Instance
	static DebugDraw* instance;

	//Reference to system core object.
	SystemCore *core;

	//Primitive items draw queue
	std::queue<Ray>			rays;
	std::queue<Grid>		grids;
	std::queue<Box>			boxes;
	std::queue<Sphere>		spheres;
	std::queue<Frustum>		frustums;
	std::queue<Cylinder>	cylinders;

	std::map<std::string, bool> groups;
	std::queue<DrawCallPayLoad> drawCalls;
	bool enabled;
	bool isWireframeEnabled;

	/// <summary>
	/// Returns true if drawing of given group is enabled. Also adds group name to group map if group name is not found in map. 
	/// </summary>
	/// <param name="groupName">Name of group to check if it is enabled</param>
	/// <returns>Boolean value indicating if group is enabled or not</returns>
	bool IsGroupEnabled(std::string groupName);

	void Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
public:
	void SetWireframeDrawEnabled(bool enabled);

	void DrawAsWireframeIfEnabled();

	/// <summary>
	/// Returns true if debug draw is enabled as a whole.
	/// </summary>
	/// <returns>Boolean representing if Debug Draw is enabled</returns>
	bool IsEnabled();

	/// <summary>
	/// Enable or disable debug draw system.
	/// </summary>
	/// <param name="enable">Boolean which enables or disables debug draw system.</param>
	void SetEnabled(bool enable);

	/// <summary>
	/// Get instance of debug draw
	/// </summary>
	/// <returns></returns>
	static DebugDraw* GetInstance();

	/// <summary>
	/// Sets the active value of the group. If true, all draw calls with given group name are rendered by debug draw system.  
	/// </summary>
	/// <param name="group">The name of the group</param>
	/// <param name="active">Boolean value to indicate if group should be active or not</param>
	void SetGroupActive(std::string group, bool active);

	/// <summary>
	/// Draw given shape type
	/// </summary>
	/// <param name="shape"></param>
	/// <param name="payload"></param>
	void Draw(PrimitiveShapesType shape, void* payload);

	/// <summary>
	/// Draws a ray according to given parameters. Will not be drawn if group is disabled
	/// </summary>
	/// <param name="ray"></param>
	/// <param name="group">Group name to associate this shape with.</param>
	void Draw(Ray ray, std::string group = "default");

	/// <summary>
	/// Draws a grid according to given parameters. Will not be drawn if group is disabled
	/// </summary>
	/// <param name="grid"></param>
	/// <param name="group">Group name to associate this shape with.</param>
	void Draw(Grid grid, std::string group = "default");

	/// <summary>
	/// Draws a sphere according to given paramters. Will not be drawn if group is disabled
	/// </summary>
	/// <param name="shape"></param>
	/// <param name="group">Group name to associate this shape with.</param>
	void Draw(Sphere shape, std::string group = "default");

	/// <summary>
	/// Draws a box according to given paramters. Will not be drawn if group is disabled
	/// </summary>
	/// <param name="shape"></param>
	/// <param name="group">Group name to associate this shape with.</param>
	void Draw(Box shape, std::string group = "default");

	/// <summary>
	/// Draws a frustum according to given paramters. Will not be drawn if group is disabled
	/// </summary>
	/// <param name="shape"></param>
	/// <param name="group">Group name to associate this shape with.</param>
	void Draw(Frustum shape, std::string group = "default");

	/// <summary>
	/// Draws a cylinder according to given paramters. Will not be drawn if group is disabled
	/// </summary>
	/// <param name="shape"></param>
	/// <param name="group">Group name to associate this shape with.</param>
	void Draw(Cylinder shape, std::string group = "default");

	/// <summary>
	/// Renders all queued up draw calls to the screen.
	/// </summary>
	/// <param name="camera">The camera object to read the view and projection matrices</param>
	void Render(Camera* camera);

	/// <summary>
	/// Instantiates debug draw system.
	/// </summary>
	/// <param name="core">System core object provides required context to the debug draw system.</param>
	DebugDraw(SystemCore* core);

	/// <summary>
	/// Generic static draw function draws any valid debug shape parameter.
	/// </summary>
	template<typename DebugDrawType>
	static void Draw(DebugDrawType debugDrawType, std::string group = "default");

	~DebugDraw();
};

/// <summary>
/// Generic static draw function draws any valid debug shape parameter. Sample -
/// <code>DebugDraw::Draw&lt;Ray&gt;(rayObject)</code>
/// </summary>
template<typename DebugDrawType>
inline void DebugDraw::Draw(DebugDrawType debugDrawType, std::string group)
{
	instance->Draw(debugDrawType, group);
}