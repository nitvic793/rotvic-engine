
// Constant Buffer
// - Allows us to define a buffer of individual variables 
//    which will (eventually) hold data from our C++ code
// - All non-pipeline variables that get their values from 
//    our C++ code must be defined inside a Constant Buffer
// - The name of the cbuffer itself is unimportant
cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	int numBones = 10;
};

struct Bones
{
	matrix BoneTransform;
	matrix InvBoneTransform;
};


cbuffer bones : register(b1)
{
	Bones bones[72];
}


struct VertexShaderInput
{ 

	float4 position		: POSITION;     // XYZ position
	float3 normal       : NORMAL;
	//float2 uv           : TEXCOORD;
	float4 boneid		: BONEID;
	float4 weight		: WEIGHT;
	//float4 color		: COLOR;        // RGBA color

};


struct VertexToPixel
{

	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal       : NORMAL;
	//float2 uv           : TEXCOORD;
	//float4 color		: COLOR;        // RGBA color
};



VertexToPixel main( VertexShaderInput input )
{

	VertexToPixel output;


	matrix worldViewProj = mul(mul(world, view), projection);

	float4 preFinalPosition = 0;
	float4 finalPosition = 0;

	matrix bonetransform = 0;
	/*
	bonetransform = mul(bones[boneids[0]], weights[0]);
	bonetransform += mul(bones[boneids[1]], weights[1]);
	bonetransform += mul(bones[boneids[2]], weights[2]);
	bonetransform += mul(bones[boneids[3]], weights[3]);
	float4 posl = mul(bonetransform, pos);
	*/
	

	if (input.boneid.x != -1)
	{
		bonetransform = mul(mul(bones[input.boneid.x].BoneTransform, input.weight.x), bones[input.boneid.x].InvBoneTransform);
	}
	if (input.boneid.y != -1)
	{
		bonetransform += mul(mul(bones[input.boneid.y].BoneTransform, input.weight.y), bones[input.boneid.y].InvBoneTransform);
	}
	if (input.boneid.z != -1)
	{
		bonetransform += mul(mul(bones[input.boneid.z].BoneTransform, input.weight.z), bones[input.boneid.z].InvBoneTransform);
	}
	if (input.boneid.w != -1)
	{
		bonetransform += mul(mul(bones[input.boneid.w].BoneTransform, input.weight.w), bones[input.boneid.w].InvBoneTransform);
	}
	
	/*
	if (input.boneid.x != -1)
	{
		preFinalPosition.x = (input.position.x * bones[input.boneid.x].BoneTransform[0][0]) + (input.position.y * bones[input.boneid.x].BoneTransform[0][1]) + (input.position.z * bones[input.boneid.x].BoneTransform[0][2]) + (input.position.w * bones[input.boneid.x].BoneTransform[0][3]); //* input.weight.x;
		preFinalPosition.y = (input.position.x * bones[input.boneid.x].BoneTransform[1][0]) + (input.position.y * bones[input.boneid.x].BoneTransform[1][1]) + (input.position.z * bones[input.boneid.x].BoneTransform[1][2]) + (input.position.w * bones[input.boneid.x].BoneTransform[1][3]);
		preFinalPosition.z = (input.position.x * bones[input.boneid.x].BoneTransform[2][0]) + (input.position.y * bones[input.boneid.x].BoneTransform[2][1]) + (input.position.z * bones[input.boneid.x].BoneTransform[2][2]) + (input.position.w * bones[input.boneid.x].BoneTransform[2][3]);
		preFinalPosition.w = 1;

		preFinalPosition.x *= input.weight.x;
		preFinalPosition.y *= input.weight.x;
		preFinalPosition.z *= input.weight.x;
	
		finalPosition.x = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[0][0]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[0][1]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[0][2]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[0][3]); //* input.weight.x;
		finalPosition.y = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[1][0]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[1][1]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[1][2]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[1][3]);
		finalPosition.z = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[2][0]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[2][1]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[2][2]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[2][3]);
		finalPosition.w = 1;
	}

	
	if (input.boneid.y != -1)
	{
		preFinalPosition.x = (finalPosition.x * bones[input.boneid.y].BoneTransform[0][0]) + (finalPosition.y * bones[input.boneid.y].BoneTransform[0][1]) + (finalPosition.z * bones[input.boneid.y].BoneTransform[0][2]) + (finalPosition.w * bones[input.boneid.y].BoneTransform[0][3]); //* input.weight.x;
		preFinalPosition.y = (finalPosition.x * bones[input.boneid.y].BoneTransform[1][0]) + (finalPosition.y * bones[input.boneid.y].BoneTransform[1][1]) + (finalPosition.z * bones[input.boneid.y].BoneTransform[1][2]) + (finalPosition.w * bones[input.boneid.y].BoneTransform[1][3]);
		preFinalPosition.z = (finalPosition.x * bones[input.boneid.y].BoneTransform[2][0]) + (finalPosition.y * bones[input.boneid.y].BoneTransform[2][1]) + (finalPosition.z * bones[input.boneid.y].BoneTransform[2][2]) + (finalPosition.w * bones[input.boneid.y].BoneTransform[2][3]);
		preFinalPosition.w = 1;

		//preFinalPosition.x *= input.weight.y;
		//preFinalPosition.y *= input.weight.y;
		//preFinalPosition.z *= input.weight.y;

		finalPosition.x = (preFinalPosition.x * bones[input.boneid.y].InvBoneTransform[0][0]) + (preFinalPosition.y * bones[input.boneid.y].InvBoneTransform[0][1]) + (preFinalPosition.z * bones[input.boneid.y].InvBoneTransform[0][2]) + (preFinalPosition.w * bones[input.boneid.y].InvBoneTransform[0][3]); //* input.weight.x;
		finalPosition.y = (preFinalPosition.x * bones[input.boneid.y].InvBoneTransform[1][0]) + (preFinalPosition.y * bones[input.boneid.y].InvBoneTransform[1][1]) + (preFinalPosition.z * bones[input.boneid.y].InvBoneTransform[1][2]) + (preFinalPosition.w * bones[input.boneid.y].InvBoneTransform[1][3]);
		finalPosition.z = (preFinalPosition.x * bones[input.boneid.y].InvBoneTransform[2][0]) + (preFinalPosition.y * bones[input.boneid.y].InvBoneTransform[2][1]) + (preFinalPosition.z * bones[input.boneid.y].InvBoneTransform[2][2]) + (preFinalPosition.w * bones[input.boneid.y].InvBoneTransform[2][3]);
		finalPosition.w = 1;
	}
	*/
	
	/*
	float4 preFinalPosition = 0;
	preFinalPosition.x = (input.position.x * bones[input.boneid.x].BoneTransform[0][0]) + (input.position.y * bones[input.boneid.x].BoneTransform[1][0]) + (input.position.z * bones[input.boneid.x].BoneTransform[2][0]) + (input.position.w * bones[input.boneid.x].BoneTransform[3][0]); //* input.weight.x;
	preFinalPosition.y = (input.position.x * bones[input.boneid.x].BoneTransform[0][1]) + (input.position.y * bones[input.boneid.x].BoneTransform[1][1]) + (input.position.z * bones[input.boneid.x].BoneTransform[2][1]) + (input.position.w * bones[input.boneid.x].BoneTransform[3][1]);
	preFinalPosition.z = (input.position.x * bones[input.boneid.x].BoneTransform[0][2]) + (input.position.y * bones[input.boneid.x].BoneTransform[1][2]) + (input.position.z * bones[input.boneid.x].BoneTransform[2][2]) + (input.position.w * bones[input.boneid.x].BoneTransform[3][2]);
	preFinalPosition.w = 1;

	
	float4 finalPosition = 0;
	finalPosition.x = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[0][0]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[1][0]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[2][0]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[3][0]); //* input.weight.x;
	finalPosition.y = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[0][1]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[1][1]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[2][1]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[3][1]);
	finalPosition.z = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[0][2]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[1][2]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[2][2]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[3][2]);
	finalPosition.w = (preFinalPosition.x * bones[input.boneid.x].InvBoneTransform[0][3]) + (preFinalPosition.y * bones[input.boneid.x].InvBoneTransform[1][3]) + (preFinalPosition.z * bones[input.boneid.x].InvBoneTransform[2][3]) + (preFinalPosition.w * bones[input.boneid.x].InvBoneTransform[3][3]);
	*/

	//finalPosition += mul(input.position, bones[input.boneid.y]); //* input.weight.y;
	//finalPosition += mul(input.position, bones[input.boneid.z]); //* input.weight.z;
	//finalPosition += mul(input.position, bones[input.boneid.w]); //* input.weight.w;
	


	output.position = mul(mul(bonetransform, input.position), worldViewProj);


	//output.color = input.color;
	output.normal = mul(mul(bonetransform, input.normal), (float3x3)world);
	//output.uv = input.uv;

	return output;
}