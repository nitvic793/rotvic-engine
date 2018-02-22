#pragma once
#include "SystemCore.h"
#include "Mesh.h"
#include "Entity.h"
#include "Constants.h"
#include "Camera.h"

/// <summary>
/// Internal render class to abstract the draw calls. 
/// </summary>
class SystemRenderer {
public:
	virtual void Draw(Mesh *mesh, ID3D11DeviceContext *context);
	virtual void SetShaders(Entity *entity, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix);
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
public:
	virtual SystemRenderer* GetInternalRenderer();
	virtual void			Draw(Mesh *mesh);
	virtual void			Draw(Entity *entity);
	virtual void			Initialize();

	XMFLOAT4X4				GetViewMatrix();
	XMFLOAT4X4				GetProjectionMatrix();
	void					SetProjectionMatrix(int width, int height);
	void					UseCamera(Camera* camera);

	int	screenHeight;
	int	screenWidth;
	Renderer(SystemCore* core, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	~Renderer();
};

