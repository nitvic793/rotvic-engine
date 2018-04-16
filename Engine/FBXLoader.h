#pragma once

#include "Mesh.h"
#include <memory>


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
		while (mAnimation)
		{
			Keyframe* temp = mAnimation->mNext;
			delete mAnimation;
			mAnimation = temp;
		}
	}
};

struct Skeleton {
	std::vector<Joint> mJoints;
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
	FbxNode* childNode;
	bool lResult;
	FbxAnimEvaluator* evaluator;
	FbxTime time;
	double T = 0.0;

	FbxAnimStack * animStack;


	Skeleton skeleton;

	FBXLoader();
	~FBXLoader();

	void InitializeSdkObjects();
	void DestroySdkObjects(bool);
	bool LoadScene(const char*);
	void LoadNodes(FbxNode*, ID3D11Device*);
	std::shared_ptr<Mesh> GetMesh(FbxNode*, ID3D11Device*);
	unsigned int FindJointIndex(const std::string &);
	void GetAnimatedMatrix();
	void GetAnimatedMatrixExtra();
	XMFLOAT4X4 GetJointGlobalTransform(int);
	XMFLOAT4X4 FbxAMatrixToXMFloat4x4(FbxAMatrix);
};

