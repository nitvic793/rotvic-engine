#include "FBXLoader.h"

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(fbxManager->GetIOSettings()))
#endif


FBXLoader::FBXLoader(FbxString name,FbxString lFilePath1, FbxString lFilePath2)
{
	meshName = name;

	InitializeSdkObjects();


	if (lFilePath1.IsEmpty())
	{
		lResult = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath1.Buffer());
		lResult = LoadScene(lFilePath1.Buffer(),scene);
		LoadScene(lFilePath2.Buffer(), scene2);
	}

	if (lResult == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
	}



	//evaluator = scene->GetAnimationEvaluator();
	//evaluator2 = scene2->GetAnimationEvaluator();
}

FBXLoader::~FBXLoader()
{
	DestroySdkObjects(lResult);
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

	scene2 = FbxScene::Create(fbxManager, "My Scene2");
	if (!scene2)
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

bool FBXLoader::LoadScene(const char* pFilename, FbxScene* pScene)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	//int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;


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
	lStatus = lImporter->Import(pScene);



	// Destroy the importer.
	lImporter->Destroy();

	return lStatus;
}





void FBXLoader::LoadNodes(FbxNode* node, std::vector<Joint>& Joints)
{
	 FbxString name = node->GetName();

	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint joint;

		
		if (Joints.size() == 0)
			joint.mParentIndex = -1;
		else
			joint.mParentIndex = Joints.size() - 1;
			

		joint.mName = node->GetName();
		Joints.push_back(joint);

	}


	FbxString nodeName = node->GetName();
	int childCount = node->GetChildCount();

	for (int i = 0; i < childCount; i++)
	{
		LoadNodes(node->GetChild(i), Joints);
	}
}


Mesh* FBXLoader::GetMesh(FbxNode * node , ID3D11Device* device)
{
	FbxString name1 = node->GetName();
	
	FbxGeometryConverter convertor(fbxManager);
	convertor.Triangulate(scene,true,false);

	if(node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{ 
		std::vector<VertexAnimated> verticesAnim;        
		std::vector<unsigned int> indices;

		FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();
		FbxVector4* controlPoints = fbxMesh->GetControlPoints();
		//int vertCounter = 0;
		unsigned int vertexCount = fbxMesh->GetControlPointsCount();
		VertexAnimated v;

		for (int i = 0; i < vertexCount; i++)
		{

			v.Position.x = (float)controlPoints[i].mData[0];
			v.Position.y = (float)controlPoints[i].mData[1];
			v.Position.z = (float)controlPoints[i].mData[2];
			v.Position.w = 1;
			v.Normal = XMFLOAT3(0, 0, 0);


			verticesAnim.push_back(v);
			//normals.push_back(XMFLOAT3(0, 0, 0));
		}

		/*
		for (int i = 8; i < vertexCount; i++)
		{
			vertices[i].Boneids.x = 1;
		}
		*/
		//int* indices_array = fbxMesh->GetPolygonVertices();

		int polygonCount = fbxMesh->GetPolygonCount();
		int polygonSize = fbxMesh->GetPolygonSize(0);
		unsigned int indexCount = polygonCount * polygonSize;

		//fbxMesh->GenerateTangentsData();
		//int tangentCount = fbxMesh->GetElementTangentCount();

		for (int i = 0; i < fbxMesh->GetPolygonCount(); i++)
		{
			for (int j = 0; j < fbxMesh->GetPolygonSize(i); j++)
			{
				int ind;
				ind = fbxMesh->GetPolygonVertex(i, j);
				indices.push_back(ind);

				FbxVector4 norm(0, 0, 0, 0);

				fbxMesh->GetPolygonVertexNormal(i, j, norm);

				verticesAnim[ind].Normal.x += norm.mData[0];
				verticesAnim[ind].Normal.y += norm.mData[1];
				verticesAnim[ind].Normal.z += norm.mData[2];

				FbxVector2 uvCoord(0, 0);
				const char* uvSet = "map1";
				bool uvFlag = true;

				if (meshName == "bee")
					uvSet = "DiffuseUV";

				fbxMesh->GetPolygonVertexUV(i, j, uvSet, uvCoord, uvFlag);

				verticesAnim[ind].UV.x = uvCoord.mData[0];
				verticesAnim[ind].UV.y = uvCoord.mData[1];
				
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
				unsigned int currJointIndex = FindJointIndex(currJointName, skeleton.mJoints);
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


					
					if (verticesAnim[index].Boneids.x == -1 && verticesAnim[index].Weights.x == -1)
					{		
							verticesAnim[index].Boneids.x = currJointIndex;
							verticesAnim[index].Weights.x = currCluster->GetControlPointWeights()[i];
					}
					else if (verticesAnim[index].Boneids.y == -1 && verticesAnim[index].Weights.y == -1)
					{
							verticesAnim[index].Boneids.y = currJointIndex;
							verticesAnim[index].Weights.y = currCluster->GetControlPointWeights()[i];
					}
					else if (verticesAnim[index].Boneids.z == -1 && verticesAnim[index].Weights.z == -1)
					{
							verticesAnim[index].Boneids.z = currJointIndex;
							verticesAnim[index].Weights.z = currCluster->GetControlPointWeights()[i];
					}
					else if (verticesAnim[index].Boneids.w == -1 && verticesAnim[index].Weights.w == -1)
					{
							verticesAnim[index].Boneids.w = currJointIndex;
							verticesAnim[index].Weights.w = currCluster->GetControlPointWeights()[i];
					}
					else
					{
						float currentWeight = currCluster->GetControlPointWeights()[i];
						//int smallestWeight = 0;

						if (verticesAnim[index].Weights.x < verticesAnim[index].Weights.y)
						{
							if (verticesAnim[index].Weights.x < verticesAnim[index].Weights.z)
							{
								if (verticesAnim[index].Weights.x < verticesAnim[index].Weights.w)
								{
									verticesAnim[index].Boneids.x = currJointIndex;
									verticesAnim[index].Weights.x = currentWeight;
								}
								else 
								{
									verticesAnim[index].Boneids.w = currJointIndex;
									verticesAnim[index].Weights.w = currentWeight;
								}
							}
							else if (verticesAnim[index].Weights.w < verticesAnim[index].Weights.z)
							{
								verticesAnim[index].Boneids.w = currJointIndex;
								verticesAnim[index].Weights.w = currentWeight;
							}
							else
							{
								verticesAnim[index].Boneids.z = currJointIndex;
								verticesAnim[index].Weights.z = currentWeight;
							}
						}
						else if (verticesAnim[index].Weights.z < verticesAnim[index].Weights.y)
						{
							if (verticesAnim[index].Weights.w < verticesAnim[index].Weights.z)
							{
								verticesAnim[index].Boneids.w = currJointIndex;
								verticesAnim[index].Weights.w = currentWeight;
							}
							else
							{
								verticesAnim[index].Boneids.z = currJointIndex;
								verticesAnim[index].Weights.z = currentWeight;
							}
						}
						else if (verticesAnim[index].Weights.w < verticesAnim[index].Weights.y)
						{
							verticesAnim[index].Boneids.w = currJointIndex;
							verticesAnim[index].Weights.w = currentWeight;
						}
						else
						{
							verticesAnim[index].Boneids.y = currJointIndex;
							verticesAnim[index].Weights.y = currentWeight;
						}


					}
					

				}
				
			}
			

		}
	
		
		//std::shared_ptr<Mesh> M(new Mesh(&vertices[0], vertexCount, &indices[0], indexCount, device));


		Mesh* M = new Mesh(&verticesAnim[0], vertexCount, &indices[0], indexCount, device);
	
		M->CalculateTangents(&verticesAnim[0], vertexCount, &indices[0], indexCount); // Caluculate Tangents

		return(M);
	}
	else
		return(NULL);
}



void FBXLoader::GetMatricesFromMesh(FbxNode * node, ID3D11Device *, std::vector<Joint>& Joints)
{
	if (node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
	{ 
		FbxMesh* fbxMesh = (FbxMesh*)node->GetNodeAttribute();
		unsigned int numOfDeformers = fbxMesh->GetDeformerCount();

		for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
		{

			FbxSkin* skin = reinterpret_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));
			unsigned int numOfClusters = skin->GetClusterCount();

			for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
			{
				FbxCluster* currCluster = skin->GetCluster(clusterIndex);
				std::string currJointName = currCluster->GetLink()->GetName();
				unsigned int currJointIndex = FindJointIndex(currJointName, Joints);
				FbxAMatrix transformMatrix;
				FbxAMatrix transformLinkMatrix;
				FbxAMatrix globalBindposeInverseMatrix;

				currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
				currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
				globalBindposeInverseMatrix = transformLinkMatrix.Inverse(); //* transformMatrix * geometryTransform;



																			 // Update the information in mSkeleton 
				globalBindposeInverseMatrix = globalBindposeInverseMatrix.Transpose();
				transformLinkMatrix = transformLinkMatrix.Transpose();

				Joints[currJointIndex].mGlobalBindposeInverse = XMFLOAT4X4(globalBindposeInverseMatrix.GetRow(0)[0], globalBindposeInverseMatrix.GetRow(0)[1], globalBindposeInverseMatrix.GetRow(0)[2], globalBindposeInverseMatrix.GetRow(0)[3], globalBindposeInverseMatrix.GetRow(1)[0], globalBindposeInverseMatrix.GetRow(1)[1], globalBindposeInverseMatrix.GetRow(1)[2], globalBindposeInverseMatrix.GetRow(1)[3], globalBindposeInverseMatrix.GetRow(2)[0], globalBindposeInverseMatrix.GetRow(2)[1], globalBindposeInverseMatrix.GetRow(2)[2], globalBindposeInverseMatrix.GetRow(2)[3], globalBindposeInverseMatrix.GetRow(3)[0], globalBindposeInverseMatrix.GetRow(3)[1], globalBindposeInverseMatrix.GetRow(3)[2], globalBindposeInverseMatrix.GetRow(3)[3]);
				Joints[currJointIndex].mNode = currCluster->GetLink();
				Joints[currJointIndex].mTransform = XMFLOAT4X4(transformLinkMatrix.GetRow(0)[0], transformLinkMatrix.GetRow(0)[1], transformLinkMatrix.GetRow(0)[2], transformLinkMatrix.GetRow(0)[3], transformLinkMatrix.GetRow(1)[0], transformLinkMatrix.GetRow(1)[1], transformLinkMatrix.GetRow(1)[2], transformLinkMatrix.GetRow(1)[3], transformLinkMatrix.GetRow(2)[0], transformLinkMatrix.GetRow(2)[1], transformLinkMatrix.GetRow(2)[2], transformLinkMatrix.GetRow(2)[3], transformLinkMatrix.GetRow(3)[0], transformLinkMatrix.GetRow(3)[1], transformLinkMatrix.GetRow(3)[2], transformLinkMatrix.GetRow(3)[3]);
				Joints[currJointIndex].mNode = currCluster->GetLink();
				Joints[currJointIndex].mFbxTransform = transformLinkMatrix;
				Joints[currJointIndex].mBoneIndex = currJointIndex;


			

			}

		}
	}
}


unsigned int FBXLoader::FindJointIndex(const std::string & jointname, std::vector<Joint>& Joints)
{
	for (int i = 0; i < Joints.size(); i++)
	{
		if (Joints[i].mName == jointname)
		{
			return i;
		}
	}
}


void FBXLoader::GetAnimatedMatrixExtra(float delTime)
{
	FbxTime repeat = 0;
	repeat.SetSecondDouble(3.3);
	time.SetSecondDouble(currentTime);
	currentTime += delTime;

	
	if(time > repeat)
	{
		currentTime -= 3.3;
		time.SetSecondDouble(currentTime);
	}
	else if (time < 0)
	{
		currentTime += 3.3;
		time.SetSecondDouble(currentTime);
	}


	for (int i = 0; i < skeleton.mJoints.size(); i++)
	{
		
		skeleton.mJoints[i].mTransform = GetJointGlobalTransform(i, skeleton.mJoints);
		skeleton.mJoints2[i].mTransform = GetJointGlobalTransform(i, skeleton.mJoints2);
	}
	

}



XMFLOAT4X4 FBXLoader::GetJointGlobalTransform(int boneIndex, std::vector<Joint>& Joints)
{
	FbxAMatrix jointTransform;
	FbxTime start;
	start.SetSecondDouble(0.0);

	FbxNode* boneNode = Joints[boneIndex].mNode;
	FbxString nameStack;
	FbxString nameLayer;
	if (boneNode)
	{
		jointTransform = boneNode->EvaluateGlobalTransform(time);
	}

	
	XMFLOAT4X4 globalTransform = FbxAMatrixToXMFloat4x4(jointTransform);
	return XMFLOAT4X4(globalTransform);
}

XMFLOAT4X4 FBXLoader::FbxAMatrixToXMFloat4x4(FbxAMatrix jointTransform)
{
	jointTransform = jointTransform.Transpose();
	return XMFLOAT4X4(jointTransform.GetRow(0)[0], jointTransform.GetRow(0)[1], jointTransform.GetRow(0)[2], jointTransform.GetRow(0)[3], jointTransform.GetRow(1)[0], jointTransform.GetRow(1)[1], jointTransform.GetRow(1)[2], jointTransform.GetRow(1)[3], jointTransform.GetRow(2)[0], jointTransform.GetRow(2)[1], jointTransform.GetRow(2)[2], jointTransform.GetRow(2)[3], jointTransform.GetRow(3)[0], jointTransform.GetRow(3)[1], jointTransform.GetRow(3)[2], jointTransform.GetRow(3)[3]);
}
