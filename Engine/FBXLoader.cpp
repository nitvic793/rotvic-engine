#include "FBXLoader.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(fbxManager->GetIOSettings()))
#endif


FBXLoader::FBXLoader()
{
	//LoadFromFile();
}

FBXLoader::~FBXLoader()
{
	DestroySdkObjects(lResult);
}

void FBXLoader::LoadFromFile(ResourceManager* resource, SystemCore *core)
{
	InitializeSdkObjects();

	FbxString lFilePath("FlexArm2.fbx");

	if (lFilePath.IsEmpty())
	{
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lFilePath.Buffer());
	}

	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}



	//FbxClassId k = FbxMesh::ClassId;


	lEvaluator = scene->GetAnimationEvaluator();

	LoadBones(scene->GetRootNode(), core->GetDevice());
	FbxNode* childNode = scene->GetRootNode()->GetChild(0);
	int k = scene->GetRootNode()->GetChildCount();
	
	GetMesh(childNode, core->GetDevice(), resource, core);
	//evaluator->SetContext(lAnimStack);
	FbxString name1 = childNode->GetName();
}

void FBXLoader::InitializeSdkObjects()
{
	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	fbxManager = FbxManager::Create();
	if (!fbxManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", fbxManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	fbxManager->LoadPluginsDirectory(lPath.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	scene = FbxScene::Create(fbxManager, "My Scene");
	if (!scene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}

void FBXLoader::DestroySdkObjects(bool pExitStatus)
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if (fbxManager) fbxManager->Destroy();
	if (pExitStatus) FBXSDK_printf("Program Success!\n");
}

bool FBXLoader::LoadScene(const char* pFilename)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(fbxManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, -1, fbxManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
		FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		FBXSDK_printf("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
		FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		FBXSDK_printf("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			FBXSDK_printf("    Animation Stack %d\n", i);
			FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			FBXSDK_printf("\n");
		}

		// Set the import states. By default, the import states are always set to 
		// true. The code below shows how to change these states.
		IOS_REF.SetBoolProp(IMP_FBX_MATERIAL, true);
		IOS_REF.SetBoolProp(IMP_FBX_TEXTURE, true);
		IOS_REF.SetBoolProp(IMP_FBX_LINK, true);
		IOS_REF.SetBoolProp(IMP_FBX_SHAPE, true);
		IOS_REF.SetBoolProp(IMP_FBX_GOBO, true);
		IOS_REF.SetBoolProp(IMP_FBX_ANIMATION, true);
		IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(scene);



	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}





void FBXLoader::LoadBones(FbxNode* node, ID3D11Device* device)
{

	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint joint;


		if (skeleton.mJoints.size() == 0)
			joint.mParentIndex = -1;
		else
			joint.mParentIndex = skeleton.mJoints.size() - 1;


		joint.mName = node->GetName();
		skeleton.mJoints.push_back(joint);

	}

	int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; i++)
	{
		LoadBones(node->GetChild(i), device);
	}
}


void FBXLoader::GetMesh(FbxNode * node, ID3D11Device* device, ResourceManager* resource,SystemCore *core)
{
	FbxString name1 = node->GetName();

	FbxGeometryConverter convertor(fbxManager);
	convertor.Triangulate(scene, true, false);

	if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		std::vector<BoneVertex> vertices;
		std::vector<unsigned int> indices;

		FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();
		FbxVector4* controlPoints = fbxMesh->GetControlPoints();

		int vertexCount = fbxMesh->GetControlPointsCount();
		BoneVertex v;

		for (int i = 0; i < vertexCount; i++)
		{

			v.Position.x = (float)controlPoints[i].mData[0];
			v.Position.y = (float)controlPoints[i].mData[1];
			v.Position.z = (float)controlPoints[i].mData[2];
			v.Position.w = 1;
			v.Normal = XMFLOAT3(0, 0, 0);


			vertices.push_back(v);
		}


		int polygonCount = fbxMesh->GetPolygonCount();
		int polygonSize = fbxMesh->GetPolygonSize(0);
		int indexCount = polygonCount * polygonSize;


		for (int i = 0; i < fbxMesh->GetPolygonCount(); i++)
		{
			for (int j = 0; j < fbxMesh->GetPolygonSize(i); j++)
			{
				int ind;
				ind = fbxMesh->GetPolygonVertex(i, j);
				indices.push_back(ind);

				FbxVector4 norm(0, 0, 0, 0);

				fbxMesh->GetPolygonVertexNormal(i, j, norm);

				vertices[ind].Normal.x += norm.mData[0];
				vertices[ind].Normal.y += norm.mData[1];
				vertices[ind].Normal.z += norm.mData[2];

			}

		}


		unsigned int numOfDeformers = fbxMesh->GetDeformerCount();

		for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
		{

			FbxSkin* skin = reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
			unsigned int numOfClusters = skin->GetClusterCount();

			for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
			{
				FbxCluster* currCluster = skin->GetCluster(clusterIndex);
				std::string currJointName = currCluster->GetLink()->GetName();
				unsigned int currJointIndex = FindJointIndex(currJointName);
				FbxAMatrix transformMatrix;
				FbxAMatrix transformLinkMatrix;
				FbxAMatrix globalBindposeInverseMatrix;

				currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
				currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse(); //* transformMatrix * geometryTransform;



																			 // Update the information in mSkeleton 
				globalBindposeInverseMatrix = globalBindposeInverseMatrix.Transpose();
				transformLinkMatrix = transformLinkMatrix.Transpose();

				skeleton.mJoints[currJointIndex].mGlobalBindposeInverse = XMFLOAT4X4(globalBindposeInverseMatrix.GetRow(0)[0], globalBindposeInverseMatrix.GetRow(0)[1], globalBindposeInverseMatrix.GetRow(0)[2], globalBindposeInverseMatrix.GetRow(0)[3], globalBindposeInverseMatrix.GetRow(1)[0], globalBindposeInverseMatrix.GetRow(1)[1], globalBindposeInverseMatrix.GetRow(1)[2], globalBindposeInverseMatrix.GetRow(1)[3], globalBindposeInverseMatrix.GetRow(2)[0], globalBindposeInverseMatrix.GetRow(2)[1], globalBindposeInverseMatrix.GetRow(2)[2], globalBindposeInverseMatrix.GetRow(2)[3], globalBindposeInverseMatrix.GetRow(3)[0], globalBindposeInverseMatrix.GetRow(3)[1], globalBindposeInverseMatrix.GetRow(3)[2], globalBindposeInverseMatrix.GetRow(3)[3]);
				skeleton.mJoints[currJointIndex].mNode = currCluster->GetLink();
				skeleton.mJoints[currJointIndex].mTransform = XMFLOAT4X4(transformLinkMatrix.GetRow(0)[0], transformLinkMatrix.GetRow(0)[1], transformLinkMatrix.GetRow(0)[2], transformLinkMatrix.GetRow(0)[3], transformLinkMatrix.GetRow(1)[0], transformLinkMatrix.GetRow(1)[1], transformLinkMatrix.GetRow(1)[2], transformLinkMatrix.GetRow(1)[3], transformLinkMatrix.GetRow(2)[0], transformLinkMatrix.GetRow(2)[1], transformLinkMatrix.GetRow(2)[2], transformLinkMatrix.GetRow(2)[3], transformLinkMatrix.GetRow(3)[0], transformLinkMatrix.GetRow(3)[1], transformLinkMatrix.GetRow(3)[2], transformLinkMatrix.GetRow(3)[3]);
				skeleton.mJoints[currJointIndex].mNode = currCluster->GetLink();
				skeleton.mJoints[currJointIndex].mFbxTransform = transformLinkMatrix;
				skeleton.mJoints[currJointIndex].mBoneIndex = currJointIndex;

				int Count = currCluster->GetControlPointIndicesCount();

				for (int i = 0; i < currCluster->GetControlPointIndicesCount(); ++i)
				{
					int index = currCluster->GetControlPointIndices()[i];
					int vertexid = indices[currCluster->GetControlPointIndices()[i]];



					if (vertices[index].Boneids.x == -1 && vertices[index].Weights.x == -1)
					{
						vertices[index].Boneids.x = currJointIndex;
						vertices[index].Weights.x = currCluster->GetControlPointWeights()[i];
					}
					else if (vertices[index].Boneids.y == -1 && vertices[index].Weights.y == -1)
					{
						vertices[index].Boneids.y = currJointIndex;
						vertices[index].Weights.y = currCluster->GetControlPointWeights()[i];
					}
					else if (vertices[index].Boneids.z == -1 && vertices[index].Weights.z == -1)
					{
						vertices[index].Boneids.z = currJointIndex;
						vertices[index].Weights.z = currCluster->GetControlPointWeights()[i];
					}
					else if (vertices[index].Boneids.w == -1 && vertices[index].Weights.w == -1)
					{
						vertices[index].Boneids.w = currJointIndex;
						vertices[index].Weights.w = currCluster->GetControlPointWeights()[i];
					}
					else
					{
						float currentWeight = currCluster->GetControlPointWeights()[i];
						//int smallestWeight = 0;

						if (vertices[index].Weights.x < vertices[index].Weights.y)
						{
							if (vertices[index].Weights.x < vertices[index].Weights.z)
							{
								if (vertices[index].Weights.x < vertices[index].Weights.w)
								{
									vertices[index].Boneids.x = currJointIndex;
									vertices[index].Weights.x = currentWeight;
								}
								else
								{
									vertices[index].Boneids.w = currJointIndex;
									vertices[index].Weights.w = currentWeight;
								}
							}
							else if (vertices[index].Weights.w < vertices[index].Weights.z)
							{
								vertices[index].Boneids.w = currJointIndex;
								vertices[index].Weights.w = currentWeight;
							}
							else
							{
								vertices[index].Boneids.z = currJointIndex;
								vertices[index].Weights.z = currentWeight;
							}
						}
						else if (vertices[index].Weights.z < vertices[index].Weights.y)
						{
							if (vertices[index].Weights.w < vertices[index].Weights.z)
							{
								vertices[index].Boneids.w = currJointIndex;
								vertices[index].Weights.w = currentWeight;
							}
							else
							{
								vertices[index].Boneids.z = currJointIndex;
								vertices[index].Weights.z = currentWeight;
							}
						}
						else if (vertices[index].Weights.w < vertices[index].Weights.y)
						{
							vertices[index].Boneids.w = currJointIndex;
							vertices[index].Weights.w = currentWeight;
						}
						else
						{
							vertices[index].Boneids.y = currJointIndex;
							vertices[index].Weights.y = currentWeight;
						}
					}

				}

			}


		}

		Mesh* m = new Mesh(core);
		m->InitializeRiggedMesh(&vertices[0], vertexCount, &indices[0], indexCount);
		resource->InsertMesh("RiggedMesh", m);

	}
}


unsigned int FBXLoader::FindJointIndex(const std::string & jointname)
{
	for (int i = 0; i < skeleton.mJoints.size(); i++)
	{
		if (skeleton.mJoints[i].mName == jointname)
		{
			return i;
		}
	}
}

void FBXLoader::GetAnimatedMatrix()
{
	//time.SetSecondDouble(0.0);
	FbxTime repeat = 0;
	repeat.SetSecondDouble(5.0);
	time.SetSecondDouble(T);
	T += 0.001;

	if (time > repeat)
	{
		T = 0;
	}



	FbxAnimLayer* lAnimLayer = animStack->GetMember<FbxAnimLayer>(0);
	FbxString lOutputString = lAnimLayer->GetName();

	for (int i = 0; i < skeleton.mJoints.size(); i++)
	{

		int index = 0;
		FbxAnimCurve* lAnimCurve;
		//FbxAnimCurve* animCurve = NULL;
		//int nbAnimLayers = animStack->GetMemberCount<FbxAnimLayer>();
		if (skeleton.mJoints[i].mNode != 0)
		{


			lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
			int numKeys = lAnimCurve->KeyGetCount();
			FbxAMatrix jointTransform = {};
			FbxVector4 vec = { 0,0,0,1 };



			/*
			for (keyIndex = 0; keyIndex < numKeys; keyIndex++)
			{
			FbxTime   lKeyTime;
			lKeyTime = lAnimCurve->KeyGetTime(1);
			if(time > )
			vec.mData[0] = 0;
			}
			*/
			//vec.mData[0] = 0;

			index = skeleton.mJoints[i].mKeyIndex;
			FbxTime keyTime = lAnimCurve->KeyGetTime(index);
			char    lTimeString[256];
			FbxString lOutputString = keyTime.GetTimeString(lTimeString, FbxUShort(256));
			FbxString currentString = time.GetTimeString(lTimeString, FbxUShort(256));

			if (time > lAnimCurve->KeyGetTime(index) && index <= numKeys)
			{
				index++;
				skeleton.mJoints[i].mKeyIndex = index;

				/*
				lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				vec.mData[0] = static_cast<float>(lAnimCurve->KeyGetValue(index));
				lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				vec.mData[1] = static_cast<float>(lAnimCurve->KeyGetValue(index));
				lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				vec.mData[2] = static_cast<float>(lAnimCurve->KeyGetValue(index));

				//vec = { 1,0,0,1 };
				jointTransform.SetT(vec);


				lAnimCurve = skeleton.mJoints[i].mNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				vec.mData[0] = static_cast<float>(lAnimCurve->KeyGetValue(index));
				lAnimCurve = skeleton.mJoints[i].mNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				vec.mData[1] = static_cast<float>(lAnimCurve->KeyGetValue(index));
				lAnimCurve = skeleton.mJoints[i].mNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				vec.mData[2] = static_cast<float>(lAnimCurve->KeyGetValue(index));

				//vec = { 0,0,0,1 };
				jointTransform.SetR(vec);


				lAnimCurve = skeleton.mJoints[i].mNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				vec.mData[0] = static_cast<float>(lAnimCurve->KeyGetValue(index));
				lAnimCurve = skeleton.mJoints[i].mNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				vec.mData[1] = static_cast<float>(lAnimCurve->KeyGetValue(index));
				lAnimCurve = skeleton.mJoints[i].mNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				vec.mData[2] = static_cast<float>(lAnimCurve->KeyGetValue(index));

				jointTransform.SetS(vec);
				int a = 0;
				*/

				lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				vec.mData[0] = 0;
				lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				vec.mData[1] = 0;
				lAnimCurve = skeleton.mJoints[i].mNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				vec.mData[2] = 0;
				vec.mData[3] = 1;

				//vec = { 1,0,0,1 };
				jointTransform.SetT(vec);


				lAnimCurve = skeleton.mJoints[i].mNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				vec.mData[0] = 0;
				lAnimCurve = skeleton.mJoints[i].mNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				vec.mData[1] = 0;
				lAnimCurve = skeleton.mJoints[i].mNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				vec.mData[2] = 0;
				vec.mData[3] = 1;

				//vec = { 0,0,0,1 };
				jointTransform.SetR(vec);


				lAnimCurve = skeleton.mJoints[i].mNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
				vec.mData[0] = 1;
				lAnimCurve = skeleton.mJoints[i].mNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
				vec.mData[1] = 1;
				lAnimCurve = skeleton.mJoints[i].mNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
				vec.mData[2] = 1;
				vec.mData[3] = 1;

				//vec = { 1,1,1,1 };


				jointTransform.SetS(vec);
				int a = 0;
			}

			//jointTransform = jointTransform.Transpose();
			jointTransform = jointTransform * skeleton.mJoints[i].mFbxTransform.Transpose();
			jointTransform = jointTransform.Transpose();

			/*
			FbxTime   lKeyTime;
			lKeyTime = lAnimCurve->KeyGetTime(1);
			char    lTimeString[256];
			FbxString lOutputString = lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
			float lKeyValue = static_cast<float>(lAnimCurve->KeyGetValue(1));
			*/
			//FbxAMatrix jointTransform = skeleton.mJoints[i].mNode->EvaluateGlobalTransform(time);
			//FbxAMatrix jointTransform = evaluator->GetNodeGlobalTransform(skeleton.mJoints[i].mNode,time);

			skeleton.mJoints[i].mTransform = XMFLOAT4X4(jointTransform.GetRow(0)[0], jointTransform.GetRow(0)[1], jointTransform.GetRow(0)[2], jointTransform.GetRow(0)[3], jointTransform.GetRow(1)[0], jointTransform.GetRow(1)[1], jointTransform.GetRow(1)[2], jointTransform.GetRow(1)[3], jointTransform.GetRow(2)[0], jointTransform.GetRow(2)[1], jointTransform.GetRow(2)[2], jointTransform.GetRow(2)[3], jointTransform.GetRow(3)[0], jointTransform.GetRow(3)[1], jointTransform.GetRow(3)[2], jointTransform.GetRow(3)[3]);
		}
	}
	//skeleton.mJoints[index].mNode->EvaluateGlobalTransform();


}

void FBXLoader::GetAnimatedMatrixExtra()
{
	FbxTime repeat = 0;
	repeat.SetSecondDouble(3.0);
	time.SetSecondDouble(T);
	T += 0.001;

	if (time > repeat)
	{
		T = 0;
	}


	for (int i = 0; i < skeleton.mJoints.size(); i++)
	{
		
		skeleton.mJoints[i].mTransform = GetJointGlobalTransform(i);

		XMMATRIX jointTransformMatrix = XMLoadFloat4x4(&skeleton.mJoints[i].mTransform);
		XMMATRIX invJointTransformMatrix = XMLoadFloat4x4(&skeleton.mJoints[i].mGlobalBindposeInverse);

		//XMMATRIX jointTransformMatrix = XMLoadFloat4x4(&animatedMatrix);

		XMFLOAT4X4 ident = {};
		XMStoreFloat4x4(&ident, jointTransformMatrix * invJointTransformMatrix);

		/*
		XMFLOAT4X4 sample = XMFLOAT4X4(1, 1, 0, 0, -1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1);
		XMMATRIX sampleMatrix = XMLoadFloat4x4(&sample);
		*/

		XMFLOAT4X4 trans = {};
		XMStoreFloat4x4(&trans, XMMatrixTranspose(jointTransformMatrix));
		bones[i].BoneTransform = trans;
		XMFLOAT4X4 trans2 = {};
		XMStoreFloat4x4(&trans2, XMMatrixTranspose(invJointTransformMatrix));
		bones[i].InvBoneTransform = trans2;
	}



}



XMFLOAT4X4 FBXLoader::GetJointGlobalTransform(int boneIndex)
{
	FbxAMatrix jointTransform;
	FbxTime start;
	start.SetSecondDouble(0.0);

	jointTransform = skeleton.mJoints[boneIndex].mNode->EvaluateGlobalTransform(time);

	//jointTransform = skeleton.mJoints[boneIndex].mNode->EvaluateLocalTransform(time);
	//jointTransform = jointTransform * skeleton.mJoints[boneIndex].mFbxTransform.Transpose();

	XMFLOAT4X4 globalTransform = FbxAMatrixToXMFloat4x4(jointTransform);

	if (skeleton.mJoints[boneIndex].mParentIndex != -1)
	{
		XMMATRIX matrix1 = XMLoadFloat4x4(&globalTransform);
		XMMATRIX matrix2 = XMLoadFloat4x4(&skeleton.mJoints[skeleton.mJoints[boneIndex].mParentIndex].mTransform);
		//jointTransform = jointTransform *skeleton.mJoints[skeleton.mJoints[boneIndex].mParentIndex].mFbxTransform.Transpose();

		matrix1 *= matrix2;
		//XMStoreFloat4x4(&globalTransform, matrix1);
	}


	return XMFLOAT4X4(globalTransform);
}

XMFLOAT4X4 FBXLoader::FbxAMatrixToXMFloat4x4(FbxAMatrix jointTransform)
{
	jointTransform = jointTransform.Transpose();
	return XMFLOAT4X4(jointTransform.GetRow(0)[0], jointTransform.GetRow(0)[1], jointTransform.GetRow(0)[2], jointTransform.GetRow(0)[3], jointTransform.GetRow(1)[0], jointTransform.GetRow(1)[1], jointTransform.GetRow(1)[2], jointTransform.GetRow(1)[3], jointTransform.GetRow(2)[0], jointTransform.GetRow(2)[1], jointTransform.GetRow(2)[2], jointTransform.GetRow(2)[3], jointTransform.GetRow(3)[0], jointTransform.GetRow(3)[1], jointTransform.GetRow(3)[2], jointTransform.GetRow(3)[3]);
}
