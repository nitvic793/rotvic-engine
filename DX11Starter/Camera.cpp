#include "Camera.h"

Camera::Camera()
{	
	position = XMFLOAT3(0, 0, -20);
	direction = XMFLOAT3(0, 0, 1);
	xRotation = 0;
	yRotation = 0;
}

Camera::~Camera()
{
}

void Camera::update()
{
	
	XMVECTOR forwardVector = XMVectorSet(direction.x, direction.y, direction.z, 0);
	forwardVector = XMVector3Rotate(forwardVector,XMQuaternionRotationRollPitchYaw(yRotation,xRotation,0));

	XMVECTOR cameraPosition = XMVectorSet(position.x,position.y,position.z, 0);
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);


	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose( XMMatrixLookToLH(cameraPosition, forwardVector, upDirection)));

}

void Camera::setPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	update();
}

void Camera::setRotation(float x, float y)
{
	xRotation = x;

	//Clamp to prevent flipping
	if (y > 0.45)
		yRotation = 0.45f;
	else if (y < -0.45)
		yRotation = -0.45f;
	else
		yRotation = y;
	update();
}

void Camera::translateTo(float x, float y, float z, float speed, float deltaTime)
{
	float x2 = position.x;
	float y2 = position.y;
	float Cos = 0, Sin = 0;
	if (x - x2 != 0 || y - y2 != 0)
	{
		Cos = (x - x2) / (sqrt((x - x2)*(x - x2) + (y - y2) * (y - y2)));
		Sin = (y - y2) / (sqrt((x - x2)*(x - x2) + (y - y2) * (y - y2)));
	}


	position.x += speed * deltaTime * Cos;
	position.y += speed * deltaTime * Sin;
	update();
}

void Camera::rotateAtSpeed(float XRotationSpeed, float YRotationSpeed, float deltaTime)
{

	setRotation(xRotation + XRotationSpeed * deltaTime, yRotation + YRotationSpeed * deltaTime);

}

void Camera::moveCameraForward(float speed, float deltaTime)
{
	XMVECTOR forwardVector = XMVectorSet(direction.x, direction.y, direction.z, 0);
	XMVECTOR moveForwardDirection = XMVector3Rotate(forwardVector, XMQuaternionRotationRollPitchYaw(yRotation,xRotation, 0));
	XMFLOAT4 vect;
	XMStoreFloat4(&vect, moveForwardDirection);

	//Finding component of camera direction along x,y,z axis 
	float xComponent = vect.x / sqrt(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
	float yComponent = vect.y / sqrt(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);
	float zComponent = vect.z / sqrt(vect.x*vect.x + vect.y*vect.y + vect.z*vect.z);

	setPosition(position.x + speed * deltaTime*xComponent, position.y + speed * deltaTime*yComponent, position.z + speed * deltaTime*zComponent);
}

void Camera::strafeCamera(float speed, float deltaTime)
{

	//Getting a perpendicular direction for strafing
	XMVECTOR upDirection = XMVectorSet(0, 1, 0, 0);
	XMVECTOR forwardVector = XMVectorSet(direction.x, direction.y, direction.z, 0);
	XMVECTOR strafeDirection = XMVector3Cross(XMVector3Rotate(forwardVector, XMQuaternionRotationRollPitchYaw(yRotation,xRotation, 0)), upDirection);
	XMFLOAT4 strafeDirectionFloat;
	XMStoreFloat4(&strafeDirectionFloat, strafeDirection);
	
	//Finding component of camera direction along x,y,z axis 
	float xComponent = strafeDirectionFloat.x / sqrt(strafeDirectionFloat.x*strafeDirectionFloat.x + strafeDirectionFloat.y*strafeDirectionFloat.y + strafeDirectionFloat.z*strafeDirectionFloat.z);
	float yComponent = strafeDirectionFloat.y / sqrt(strafeDirectionFloat.x*strafeDirectionFloat.x + strafeDirectionFloat.y*strafeDirectionFloat.y + strafeDirectionFloat.z*strafeDirectionFloat.z);
	float zComponent = strafeDirectionFloat.z / sqrt(strafeDirectionFloat.x*strafeDirectionFloat.x + strafeDirectionFloat.y*strafeDirectionFloat.y + strafeDirectionFloat.z*strafeDirectionFloat.z);

	
	setPosition(position.x + speed * deltaTime*xComponent, position.y + speed * deltaTime*yComponent, position.z + speed * deltaTime*zComponent);
}

void Camera::raiseCamera(float speed, float deltaTime)
{
	position.y += speed * deltaTime;
	update();
}

void Camera::updateProjectionMatrix(float aspectRatio)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		aspectRatio,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); 
}

XMFLOAT4X4 Camera::getViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::getProjectionMatrix()
{
	return projectionMatrix;
}

XMFLOAT3 Camera::getPosition()
{
	return position;
}

XMFLOAT3 Camera::getDirection()
{
	return direction;
}

float Camera::getXRotation()
{
	return xRotation;
}

float Camera::getYRotation()
{
	return yRotation;
}
