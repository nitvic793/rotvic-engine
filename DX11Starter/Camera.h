#pragma once
#include <stdio.h>
#include <memory>
#include <math.h>  
#include <iostream>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;


class Camera
{
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT3 position;
	XMFLOAT3 direction;
	float xRotation;
	float yRotation;

public:

	Camera();
	~Camera();

	void update();
	void setPosition(float, float, float);
	void setRotation(float, float);
	void translateTo(float,float,float,float,float);
	void rotateAtSpeed(float, float, float);
	void moveCameraForward(float, float);
	void strafeCamera(float, float);
	void raiseCamera(float, float);
	void updateProjectionMatrix(float);


	XMFLOAT4X4 getViewMatrix();
	XMFLOAT4X4 getProjectionMatrix();
	XMFLOAT3 getPosition();
	XMFLOAT3 getDirection();
	float getXRotation();
	float getYRotation();

};
