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

void DebugDraw::Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(indexCount, 0, 0);
}


void DebugDraw::Draw(PrimitiveShapesType shape, Transform transform)
{
	drawCalls.push({ shape, transform });
}

void DebugDraw::Render(Camera* camera)
{
	auto context = core->GetDeviceContext();
	//Set output merger and rasterizer options.
	context->OMSetBlendState(states->Opaque(), nullptr, 0xFFFFFFFF);
	//context->OMSetDepthStencilState(states->DepthNone(), 0);
	context->RSSetState(states->CullNone());

	auto rm = ResourceManager::GetInstance();
	auto ps = rm->debugShader;
	auto vs = rm->vertexShader;
	cube->transform.SetScale(3, 1, 1);
	cube->transform.SetPosition(2, 1, 5);
	cube->transform.SetRotation(10, 30, 0);

	vs->SetMatrix4x4(WORLD_STR, cube->transform.GetWorldMatrix());
	vs->SetMatrix4x4(VIEW_STR, camera->GetViewMatrix());
	vs->SetMatrix4x4(PROJECTION_STR, camera->GetProjectionMatrix());
	vs->CopyAllBufferData();
	ps->CopyAllBufferData();
	ps->SetShader();
	vs->SetShader();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //Ensure only lines are drawn
	Draw(core->GetDeviceContext(), cube->vertexBuffer, cube->indexBuffer, cube->indexCount);

	//Reset Pipeline 
	context->OMSetBlendState(0, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState(0, 0);
	context->RSSetState(nullptr);
}

DebugDraw::DebugDraw(SystemCore* core)
{
	this->core = core;
	cube = PrimitiveShape::InstantiateCube(core);
	shapeBuffers.insert(std::pair<PrimitiveShapesType, PrimitiveShape*>(CUBE,cube));
	if (shapeBuffers.find(CONE) != shapeBuffers.end()) 
	{
		printf("Test");
	}
	states = std::make_unique<CommonStates>(core->GetDevice());
}


DebugDraw::~DebugDraw()
{
	delete cube;
}
