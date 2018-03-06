#include "DebugDraw.h"
#include "Entity.h"

void XM_CALLCONV DrawRing(PrimitiveBatch<VertexPositionColor>* batch,
	FXMVECTOR origin,
	FXMVECTOR majorAxis,
	FXMVECTOR minorAxis,
	GXMVECTOR color)
{
	static const size_t c_ringSegments = 32;

	VertexPositionColor verts[c_ringSegments + 1];

	FLOAT fAngleDelta = XM_2PI / float(c_ringSegments);
	// Instead of calling cos/sin for each segment we calculate
	// the sign of the angle delta and then incrementally calculate sin
	// and cosine from then on.
	XMVECTOR cosDelta = XMVectorReplicate(cosf(fAngleDelta));
	XMVECTOR sinDelta = XMVectorReplicate(sinf(fAngleDelta));
	XMVECTOR incrementalSin = XMVectorZero();
	static const XMVECTORF32 s_initialCos =
	{
		1.f, 1.f, 1.f, 1.f
	};
	XMVECTOR incrementalCos = s_initialCos.v;
	for (size_t i = 0; i < c_ringSegments; i++)
	{
		XMVECTOR pos = XMVectorMultiplyAdd(majorAxis, incrementalCos, origin);
		pos = XMVectorMultiplyAdd(minorAxis, incrementalSin, pos);
		XMStoreFloat3(&verts[i].position, pos);
		XMStoreFloat4(&verts[i].color, color);
		// Standard formula to rotate a vector.
		XMVECTOR newCos = incrementalCos * cosDelta - incrementalSin * sinDelta;
		XMVECTOR newSin = incrementalCos * sinDelta + incrementalSin * cosDelta;
		incrementalCos = newCos;
		incrementalSin = newSin;
	}
	verts[c_ringSegments] = verts[0];

	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, c_ringSegments + 1);
	//batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, verts, c_ringSegments + 1);
}


void XM_CALLCONV DebugDraw::Draw(const BoundingSphere & sphere, FXMVECTOR color)
{
	XMVECTOR origin = XMLoadFloat3(&sphere.Center);

	const float radius = sphere.Radius;

	XMVECTOR xaxis = g_XMIdentityR0 * radius;
	XMVECTOR yaxis = g_XMIdentityR1 * radius;
	XMVECTOR zaxis = g_XMIdentityR2 * radius;

	DrawRing(batch.get(), origin, xaxis, zaxis, color);
	DrawRing(batch.get(), origin, xaxis, yaxis, color);
	DrawRing(batch.get(), origin, yaxis, zaxis, color);
	return;
}

void Initialize(Vertex *vertices, UINT vertexCount, UINT *indices, UINT indexCount, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, ID3D11Device* device)
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = vertices;

	device->CreateBuffer(&vbd, &initialVertexData, &*vertexBuffer);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * indexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = indices;

	device->CreateBuffer(&ibd, &initialIndexData, &*indexBuffer);
}

void DebugDraw::Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(indexCount, 0, 0);
}



inline void XM_CALLCONV DebugDraw::DrawCube(SystemCore* core,
	CXMMATRIX matWorld)
{
	static const XMVECTORF32 s_verts[8] =
	{
		{ -1.f, -1.f, -1.f, 0.f },
		{ 1.f, -1.f, -1.f, 0.f },
		{ 1.f, -1.f,  1.f, 0.f },
		{ -1.f, -1.f,  1.f, 0.f },
		{ -1.f,  1.f, -1.f, 0.f },
		{ 1.f,  1.f, -1.f, 0.f },
		{ 1.f,  1.f,  1.f, 0.f },
		{ -1.f,  1.f,  1.f, 0.f }
	};

	static UINT s_indices[] =
	{
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 4,
		1, 5,
		2, 6,
		3, 7
	}; //12

	Vertex verts[8];
	for (size_t i = 0; i < 8; ++i)
	{
		verts[i] = {};
		XMVECTOR v = XMVector3Transform(s_verts[i], matWorld);
		XMStoreFloat3(&verts[i].Position, v);
	}

	if (cubeIndexBuffer == nullptr)
		Initialize(verts, 8, s_indices, 24, &cubeVertexBuffer, &cubeIndexBuffer, core->GetDevice());
	Draw(core->GetDeviceContext(), cubeVertexBuffer, cubeIndexBuffer, 24);
}


void DebugDraw::Render(Camera* camera)
{
	auto context = core->GetDeviceContext();
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());
	//effect->SetView(XMLoadFloat4x4(&camera->GetViewMatrix()));
	//effect->SetProjection(XMLoadFloat4x4(&camera->GetProjectionMatrix()));
	//effect->Apply(context);

	//context->IASetInputLayout(inputLayout.Get());
	auto rm = ResourceManager::GetInstance();
	auto ps = rm->debugShader;
	auto vs = rm->vertexShader;
	Entity* entity = new Entity();
	entity->SetPosition(0, 0, 0);
	auto identity = XMMatrixIdentity();
	XMFLOAT4X4 mat;
	auto m = entity->GetWorldMatrix();
	XMStoreFloat4x4(&mat, XMMatrixTranspose(identity));
	vs->SetMatrix4x4(WORLD_STR, m);

	//batch->Begin();
	vs->SetMatrix4x4(VIEW_STR, camera->GetViewMatrix());
	vs->SetMatrix4x4(PROJECTION_STR, camera->GetProjectionMatrix());
	vs->CopyAllBufferData();
	ps->CopyAllBufferData();
	ps->SetShader();
	vs->SetShader();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	DrawCube(core, identity);

	delete entity;
	context->OMSetBlendState(0, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState(0, 0);
	context->RSSetState(nullptr);
}

DebugDraw::DebugDraw(SystemCore* core)
{

	this->core = core;
	states = std::make_unique<CommonStates>(core->GetDevice());
	effect = std::make_unique<BasicEffect>(core->GetDevice());
	effect->SetVertexColorEnabled(true);

	{
		void const* shaderByteCode;
		size_t byteCodeLength;

		effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
		core->GetDevice()->CreateInputLayout(
			VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
			shaderByteCode, byteCodeLength,
			inputLayout.ReleaseAndGetAddressOf());
	}

	batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(core->GetDeviceContext());
}


DebugDraw::~DebugDraw()
{
	if (cubeVertexBuffer)cubeVertexBuffer->Release();
	if (cubeIndexBuffer)cubeIndexBuffer->Release();
}
