
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	// No world matrix necessary for the sky box
	matrix view;
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 sampleDir	: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	matrix viewNoTranslation = view;
	viewNoTranslation._41 = 0;
	viewNoTranslation._42 = 0;
	viewNoTranslation._43 = 0;

	// Calculate output position
	matrix viewProj = mul(viewNoTranslation, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);
	output.position.z = output.position.w;

	// Which direction is the vertex?
	// Assume that the vert's raw position in space is also
	// it's direction from the origin (this works if the model is
	// actually centered around the origin)
	output.sampleDir = input.position;

	return output;
}