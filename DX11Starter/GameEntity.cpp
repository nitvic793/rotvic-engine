#include "GameEntity.h"

GameEntity::GameEntity(std::shared_ptr<Mesh> newMesh, std::shared_ptr<Material> newMaterial)
{
	mesh = newMesh;
	material = newMaterial;

	XMMATRIX identitiy = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(identitiy));

	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	
	// Animation Initialization
	/*
	lSdkManager = mesh->lSdkManager;
	childNode = mesh->childNode;
	scene = mesh->scene;
	lEvaluator = scene->GetAnimationEvaluator();
	time.SetSecondDouble(0.0);
	*/
}


GameEntity::~GameEntity()
{
	
}

void GameEntity::prepareMaterial(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	material->getVertexShader()->SetMatrix4x4("world", worldMatrix);
	material->getVertexShader()->SetMatrix4x4("view", viewMatrix);
	material->getVertexShader()->SetMatrix4x4("projection", projectionMatrix);

	material->getVertexShader()->CopyAllBufferData();
	material->getPixelShader()->CopyAllBufferData();
	material->getVertexShader()->SetShader();
	material->getPixelShader()->SetShader();
}

XMFLOAT4X4 GameEntity::getWorldMatrix()
{
	return worldMatrix;
}

XMFLOAT3 GameEntity::getPosition()
{
	return position;
}

XMFLOAT3 GameEntity::getRotation()
{
	return rotation;
}

XMFLOAT3 GameEntity::getScale()
{
	return scale;
}

std::shared_ptr<Mesh> GameEntity::getMesh()
{
	return mesh;
}

void GameEntity::updateWorldMatrix()
{
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);

	XMMATRIX Trans = XMMatrixTranslation(position.x,position.y,position.z);
	XMMATRIX Rot = XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z);
	XMMATRIX Scale = XMMatrixScaling(scale.x, scale.y, scale.z);

	world = XMMatrixTranspose(Scale* Rot *  Trans);

	XMStoreFloat4x4(&worldMatrix,world);
}

void GameEntity::setPosition(float x,float y,float z)
{
	position = XMFLOAT3(x,y,z);
	updateWorldMatrix();
}

void GameEntity::setRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	updateWorldMatrix();
}

void GameEntity::setScale(float x, float y, float z)
{
	scale = XMFLOAT3(x, y, z);
	updateWorldMatrix();
}

void GameEntity::translateTo(float x, float y, float z, float speed, float deltaTime)
{
		float x2 = position.x;
		float y2 = position.y;
		float Cos = 0, Sin = 0;
		if (x - x2 != 0 || y - y2 != 0)
		{
			Cos = (x - x2) / (sqrt((x - x2)*(x - x2) + (y - y2) * (y - y2)));
			Sin = (y - y2) / (sqrt((x - x2)*(x - x2) + (y - y2) * (y - y2)));
		}
	

		//2D translation at the moment
		position.x += speed * deltaTime * Cos;
		position.y += speed * deltaTime * Sin;
		updateWorldMatrix();	
}

void GameEntity::rotateAt(float XRotationSpeed, float YRotationSpeed, float ZRotationSpeed, float deltaTime)
{
	float x = rotation.x; 
	float y = rotation.y;
	float z = rotation.z;
	setRotation(x+XRotationSpeed*deltaTime,y+YRotationSpeed*deltaTime,z+ ZRotationSpeed*deltaTime);
	
}
/*
XMFLOAT4X4 GameEntity::GetAnimatedMatrix()
{
	//FbxVector4 vec(0,0,0,0);
	
	FbxAMatrix animatedMatrix;
	
	time.SetSecondDouble(0.0);

	T+= 0.001;

	//time += increment;
	time.SetSecondDouble(T);
	animatedMatrix = lEvaluator->GetNodeLocalTransform(childNode, time);
	//vec = childNode->EvaluateLocalRotation(time);


	//setRotation(vec.mData[0],vec.mData[1], vec.mData[2]);

	XMFLOAT4X4 final = XMFLOAT4X4(animatedMatrix.GetRow(0)[0], animatedMatrix.GetRow(0)[1], animatedMatrix.GetRow(0)[2], animatedMatrix.GetRow(0)[3], animatedMatrix.GetRow(1)[0], animatedMatrix.GetRow(1)[1], animatedMatrix.GetRow(1)[2], animatedMatrix.GetRow(1)[3], animatedMatrix.GetRow(2)[0], animatedMatrix.GetRow(2)[1], animatedMatrix.GetRow(2)[2], animatedMatrix.GetRow(2)[3], animatedMatrix.GetRow(3)[0], animatedMatrix.GetRow(3)[1], animatedMatrix.GetRow(3)[2], animatedMatrix.GetRow(3)[3]);
	
	return(final);
	
}

*/