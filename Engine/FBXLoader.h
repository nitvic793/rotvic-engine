#pragma once

#include "Mesh.h"
#include <fbxsdk.h>


/* Joints are the transforms that make a skeleton */
struct Joint {
public:
	int mBoneIndex;
	int mParentIndex;
	const char* mName;
	DirectX::XMFLOAT4X4 mGlobalBindposeInverse = {};
	FbxAMatrix mFbxTransform = {};
	DirectX::XMFLOAT4X4 mTransform = {};
	
	FbxNode *mNode;
	int mKeyIndex = 0;

	Joint() :
		mNode(nullptr)
	{
		//mGlobalBindposeInverse.XMMatrixIdentity();
		mBoneIndex = -1;
	}

	~Joint()
	{

	}
};

/* Collection of joints used for animation */
struct Skeleton {
	std::vector<Joint> mJoints;
	std::vector<Joint> mJoints2;
};

/* Struct holding the transform and inverse bind pose transform of a joint */
struct Bones
{
	DirectX::XMFLOAT4X4 BoneTransform = {};
	DirectX::XMFLOAT4X4 InvBoneTransform = {};
};


class FBXLoader 
{
public:
	FbxString meshName;
	FbxManager* fbxManager = NULL;

	FbxScene* scene = NULL;
	FbxScene* scene2 = NULL;

	bool lResult;
	
	FbxTime time;
	double currentTime = 0.0;

	FbxAnimStack * animStack;


	Skeleton skeleton;
	float blendWeight = 1;

	FBXLoader(FbxString, FbxString, FbxString);
	~FBXLoader();

	void InitializeSdkObjects();
	void DestroySdkObjects(bool);
	bool LoadScene(const char*, FbxScene*);
	void LoadNodes(FbxNode*, std::vector<Joint>&);
	Mesh* GetMesh(FbxNode*, ID3D11Device*);
	void GetMatricesFromMesh(FbxNode*, ID3D11Device*, std::vector<Joint>&);
	unsigned int FindJointIndex(const std::string &, std::vector<Joint>& Joints);
	void GetAnimatedMatrixExtra(float delTime);
	XMFLOAT4X4 GetJointGlobalTransform(int , std::vector<Joint>&);
	XMFLOAT4X4 FbxAMatrixToXMFloat4x4(FbxAMatrix);
};

