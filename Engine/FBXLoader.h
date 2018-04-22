#pragma once

#include "Mesh.h"
#include <fbxsdk.h>

struct Keyframe {
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	Keyframe* mNext;

	Keyframe() : mNext(nullptr)
	{}
};

struct Joint {
public:
	int mBoneIndex;
	int mParentIndex;
	const char* mName;
	DirectX::XMFLOAT4X4 mGlobalBindposeInverse = {};
	FbxAMatrix mFbxTransform = {};
	DirectX::XMFLOAT4X4 mTransform = {};
	Keyframe* mAnimation;
	FbxNode *mNode;
	int mKeyIndex = 0;

	Joint() :
		mNode(nullptr),
		mAnimation(nullptr)
	{
		//mGlobalBindposeInverse.XMMatrixIdentity();
		mBoneIndex = -1;
	}

	~Joint()
	{

	}
};

struct Skeleton {
	std::vector<Joint> mJoints;
	std::vector<Joint> mJoints2;
};

struct Bones
{
	DirectX::XMFLOAT4X4 BoneTransform = {};
	DirectX::XMFLOAT4X4 InvBoneTransform = {};
};


class FBXLoader 
{
public:

	FbxManager* fbxManager = NULL;

	FbxScene* scene = NULL;
	FbxScene* scene2 = NULL;

	//FbxAnimEvaluator* evaluator;
	//FbxAnimEvaluator* evaluator2;

	FbxNode* childNode;
	bool lResult;
	
	FbxTime time;
	double T = 0.0;

	FbxAnimStack * animStack;


	Skeleton skeleton;

	FBXLoader();
	~FBXLoader();

	void InitializeSdkObjects();
	void DestroySdkObjects(bool);
	bool LoadScene(const char*, FbxScene*);
	void LoadNodes(FbxNode*, std::vector<Joint>&);
	Mesh* GetMesh(FbxNode*, ID3D11Device*);
	void GetMatricesFromMesh(FbxNode*, ID3D11Device*, std::vector<Joint>&);
	unsigned int FindJointIndex(const std::string &, std::vector<Joint>& Joints);
	void GetAnimatedMatrix();
	void GetAnimatedMatrixExtra();
	XMFLOAT4X4 GetJointGlobalTransform(int , std::vector<Joint>&);
	XMFLOAT4X4 FbxAMatrixToXMFloat4x4(FbxAMatrix);
};

