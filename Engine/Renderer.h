#pragma once
#include "SystemCore.h"
#include "Mesh.h"
#include "Entity.h"
#include "Constants.h"
#include "Camera.h"
#include "Lights.h"
#include "Skybox.h"

/// <summary>
/// Internal render class to abstract the draw calls. 
/// </summary>
class SystemRenderer {
	D3D11_PRIMITIVE_TOPOLOGY primitiveTopology;
public:
	virtual void Draw(Mesh *mesh, ID3D11DeviceContext *context);
	virtual void SetShaders(Entity *entity, Camera* camera, LightsMap lights);
	void SetShadersAndDrawAnimated(Entity *entity, Camera* camera, LightsMap lights, ID3D11DeviceContext *context);
};

/// <summary>
/// Renderer class is responsible for rendering all the game entitites present in the game. 
/// </summary>
class Renderer
{
	SystemRenderer	*internalRenderer;
	SystemCore		*core;
	XMFLOAT4X4		viewMatrix;
	XMFLOAT4X4		projectionMatrix;
	Camera*			camera;
	LightsMap		lights;
public:
	virtual SystemRenderer* GetInternalRenderer();
	virtual void			Draw(Mesh *mesh);
	virtual void			Draw(Entity *entity);
	virtual void			Draw(Skybox* sky);
	virtual void			Initialize();

	XMFLOAT4X4				GetViewMatrix();
	XMFLOAT4X4				GetProjectionMatrix();
	void					SetProjectionMatrix(int width, int height);
	void					UseCamera(Camera* camera);
	void					SetLights(LightsMap lightsMap);
	void					ClearScreen();
	void					SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY primitive);
	void					Present();

	int	screenHeight;
	int	screenWidth;
	Renderer(SystemCore* core, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	~Renderer();
};

