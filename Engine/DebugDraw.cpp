/// <summary>
/// Author: Nitish Victor
/// Heavily referenced from: https://github.com/Microsoft/DirectXTK/wiki/DebugDraw
/// All content and source code for DirectXTK package are subject to the terms of the MIT License (https://opensource.org/licenses/MIT).
/// </summary>


#include "DebugDraw.h"
#include "Entity.h"
#include "Mouse.h"

DebugDraw* DebugDraw::instance = nullptr;

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

void XM_CALLCONV DrawTriangle(PrimitiveBatch<VertexPositionColor>* batch,
	FXMVECTOR pointA,
	FXMVECTOR pointB,
	FXMVECTOR pointC,
	GXMVECTOR color)
{
	VertexPositionColor verts[4];
	XMStoreFloat3(&verts[0].position, pointA);
	XMStoreFloat3(&verts[1].position, pointB);
	XMStoreFloat3(&verts[2].position, pointC);
	XMStoreFloat3(&verts[3].position, pointA);

	XMStoreFloat4(&verts[0].color, color);
	XMStoreFloat4(&verts[1].color, color);
	XMStoreFloat4(&verts[2].color, color);
	XMStoreFloat4(&verts[3].color, color);

	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, 4);
}

void XM_CALLCONV DrawGrid(PrimitiveBatch<VertexPositionColor>* batch,
	FXMVECTOR xAxis,
	FXMVECTOR yAxis,
	FXMVECTOR origin,
	size_t xdivs,
	size_t ydivs,
	GXMVECTOR color)
{
	xdivs = std::max<size_t>(1, xdivs);
	ydivs = std::max<size_t>(1, ydivs);

	for (size_t i = 0; i <= xdivs; ++i)
	{
		float percent = float(i) / float(xdivs);
		percent = (percent * 2.f) - 1.f;
		XMVECTOR scale = XMVectorScale(xAxis, percent);
		scale = XMVectorAdd(scale, origin);

		VertexPositionColor v1(XMVectorSubtract(scale, yAxis), color);
		VertexPositionColor v2(XMVectorAdd(scale, yAxis), color);
		batch->DrawLine(v1, v2);
	}

	for (size_t i = 0; i <= ydivs; i++)
	{
		FLOAT percent = float(i) / float(ydivs);
		percent = (percent * 2.f) - 1.f;
		XMVECTOR scale = XMVectorScale(yAxis, percent);
		scale = XMVectorAdd(scale, origin);

		VertexPositionColor v1(XMVectorSubtract(scale, xAxis), color);
		VertexPositionColor v2(XMVectorAdd(scale, xAxis), color);
		batch->DrawLine(v1, v2);
	}
}

void XM_CALLCONV DrawRay(PrimitiveBatch<VertexPositionColor>* batch,
	FXMVECTOR origin,
	FXMVECTOR direction,
	bool normalize,
	FXMVECTOR color)
{
	VertexPositionColor verts[3];
	XMStoreFloat3(&verts[0].position, origin);

	XMVECTOR normDirection = XMVector3Normalize(direction);
	XMVECTOR rayDirection = (normalize) ? normDirection : direction;

	XMVECTOR perpVector = XMVector3Cross(normDirection, g_XMIdentityR1);

	if (XMVector3Equal(XMVector3LengthSq(perpVector), g_XMZero))
	{
		perpVector = XMVector3Cross(normDirection, g_XMIdentityR2);
	}
	perpVector = XMVector3Normalize(perpVector);

	XMStoreFloat3(&verts[1].position, XMVectorAdd(rayDirection, origin));
	perpVector = XMVectorScale(perpVector, 0.0625f);
	normDirection = XMVectorScale(normDirection, -0.25f);
	rayDirection = XMVectorAdd(perpVector, rayDirection);
	rayDirection = XMVectorAdd(normDirection, rayDirection);
	XMStoreFloat3(&verts[2].position, XMVectorAdd(rayDirection, origin));

	XMStoreFloat4(&verts[0].color, color);
	XMStoreFloat4(&verts[1].color, color);
	XMStoreFloat4(&verts[2].color, color);

	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP, verts, 2);
}

void XM_CALLCONV DrawCylinder(PrimitiveBatch<VertexPositionColor>* batch,
	FXMVECTOR origin,
	const float height,
	const float radius,
	CXMMATRIX matWorld,
	FXMVECTOR color)
{
	XMVECTOR xaxis = g_XMIdentityR0 * radius;
	XMVECTOR zaxis = g_XMIdentityR2 * radius;

	XMVECTOR top = XMVectorAdd(origin, XMVectorSet(0, height / 2.f, 0, 0));
	XMVECTOR leftTop = XMVectorAdd(origin, XMVectorSet(-radius, height / 2.f, 0, 0));
	XMVECTOR rightTop = XMVectorAdd(origin, XMVectorSet(radius, height / 2.f, 0, 0));

	XMVECTOR frontTop = XMVectorAdd(origin, XMVectorSet(0, height / 2.f, radius, 0));
	XMVECTOR backTop = XMVectorAdd(origin, XMVectorSet(0, height / 2.f, -radius, 0));

	XMVECTOR frontBottom = XMVectorAdd(origin, XMVectorSet(0, -(height / 2.f), radius, 0));
	XMVECTOR backBottom = XMVectorAdd(origin, XMVectorSet(0, -(height / 2.f), -radius, 0));

	XMVECTOR bottom = XMVectorAdd(origin, XMVectorSet(0, -height / 2.f, 0, 0));
	XMVECTOR leftBottom = XMVectorAdd(origin, XMVectorSet(-radius, -height / 2.f, 0, 0));
	XMVECTOR rightBottom = XMVectorAdd(origin, XMVectorSet(radius, -height / 2.f, 0, 0));

	VertexPositionColor vertsLeft[2];
	VertexPositionColor vertsFront[2];
	VertexPositionColor vertsBack[2];
	VertexPositionColor vertsRight[2];
	VertexPositionColor vertsBottom[2];
	VertexPositionColor vertsTop[2];
	std::vector<VertexPositionColor> verts;

	XMStoreFloat3(&vertsLeft[0].position, leftTop);
	XMStoreFloat3(&vertsLeft[1].position, leftBottom);
	XMStoreFloat4(&vertsLeft[0].color, color);
	XMStoreFloat4(&vertsLeft[1].color, color);
	verts.push_back(vertsLeft[0]);
	verts.push_back(vertsLeft[1]);

	XMStoreFloat3(&vertsTop[0].position, leftTop);
	XMStoreFloat3(&vertsTop[1].position, rightTop);
	XMStoreFloat4(&vertsTop[0].color, color);
	XMStoreFloat4(&vertsTop[1].color, color);
	verts.push_back(vertsTop[0]);
	verts.push_back(vertsTop[1]);

	XMStoreFloat3(&vertsRight[0].position, rightTop);
	XMStoreFloat3(&vertsRight[1].position, rightBottom);
	XMStoreFloat4(&vertsRight[0].color, color);
	XMStoreFloat4(&vertsRight[1].color, color);
	verts.push_back(vertsRight[0]);
	verts.push_back(vertsRight[1]);

	XMStoreFloat3(&vertsBottom[0].position, leftBottom);
	XMStoreFloat3(&vertsBottom[1].position, rightBottom);
	XMStoreFloat4(&vertsBottom[0].color, color);
	XMStoreFloat4(&vertsBottom[1].color, color);
	verts.push_back(vertsBottom[0]);
	verts.push_back(vertsBottom[1]);

	XMStoreFloat3(&vertsBack[0].position, backTop);
	XMStoreFloat3(&vertsBack[1].position, backBottom);
	XMStoreFloat4(&vertsBack[0].color, color);
	XMStoreFloat4(&vertsBack[1].color, color);
	verts.push_back(vertsBack[0]);
	verts.push_back(vertsBack[1]);

	XMStoreFloat3(&vertsFront[0].position, frontTop);
	XMStoreFloat3(&vertsFront[1].position, frontBottom);
	XMStoreFloat4(&vertsFront[0].color, color);
	XMStoreFloat4(&vertsFront[1].color, color);
	verts.push_back(vertsFront[0]);
	verts.push_back(vertsFront[1]);

	for (int i = 0; i < verts.size(); ++i)
	{
		XMVECTOR v = XMVector3Transform(XMLoadFloat3(&verts[i].position), matWorld);
		XMStoreFloat3(&verts[i].position, v);
		XMStoreFloat4(&verts[i].color, color);
	}

	static const WORD indices[] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	//batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, indices, 12, verts.data(), verts.size());
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertsLeft, 2);
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertsRight, 2);
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertsBack, 2);
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertsFront, 2);
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertsBottom, 2);
	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, vertsTop, 2);
	DrawRing(batch, top, xaxis, zaxis, color);
	DrawRing(batch, bottom, xaxis, zaxis, color);
}

void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
	const BoundingSphere& sphere,
	FXMVECTOR color)
{
	XMVECTOR origin = XMLoadFloat3(&sphere.Center);

	const float radius = sphere.Radius;

	XMVECTOR xaxis = g_XMIdentityR0 * radius;
	XMVECTOR yaxis = g_XMIdentityR1 * radius;
	XMVECTOR zaxis = g_XMIdentityR2 * radius;

	DrawRing(batch, origin, xaxis, zaxis, color);
	DrawRing(batch, origin, xaxis, yaxis, color);
	DrawRing(batch, origin, yaxis, zaxis, color);
}

inline void XM_CALLCONV DrawCube(PrimitiveBatch<VertexPositionColor>* batch,
	CXMMATRIX matWorld,
	FXMVECTOR color)
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

	static const WORD s_indices[] =
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
	};

	VertexPositionColor verts[8];
	for (size_t i = 0; i < 8; ++i)
	{
		XMVECTOR v = XMVector3Transform(s_verts[i], matWorld);
		XMStoreFloat3(&verts[i].position, v);
		XMStoreFloat4(&verts[i].color, color);
	}

	batch->DrawIndexed(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, s_indices, _countof(s_indices), verts, 8);
}


void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
	const BoundingBox& box,
	FXMVECTOR color)
{
	XMMATRIX matWorld = XMMatrixScaling(box.Extents.x, box.Extents.y, box.Extents.z);
	XMVECTOR position = XMLoadFloat3(&box.Center);
	matWorld.r[3] = XMVectorSelect(matWorld.r[3], position, g_XMSelect1110);
	DrawCube(batch, matWorld, color);
}

void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
	const Cylinder& cylinder)
{
	XMVECTOR position = XMLoadFloat4(&cylinder.Center);
	XMMATRIX matWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&cylinder.Orientation));
	//XMMATRIX transform = XMMatrixTranslationFromVector(position);
	//matWorld = XMMatrixMultiply(matWorld, transform);
	
	//matWorld.r[3] = XMVectorSelect(matWorld.r[3], position, g_XMSelect1110);
	DrawCylinder(batch, XMLoadFloat4(&cylinder.Center), cylinder.Height, cylinder.Radius, matWorld, XMLoadFloat4(&cylinder.color));
}


void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
	const BoundingOrientedBox& obb,
	FXMVECTOR color)
{
	XMMATRIX matWorld = XMMatrixRotationQuaternion(XMLoadFloat4(&obb.Orientation));
	XMMATRIX matScale = XMMatrixScaling(obb.Extents.x, obb.Extents.y, obb.Extents.z);
	matWorld = XMMatrixMultiply(matScale, matWorld);
	XMVECTOR position = XMLoadFloat3(&obb.Center);
	matWorld.r[3] = XMVectorSelect(matWorld.r[3], position, g_XMSelect1110);

	DrawCube(batch, matWorld, color);
}


void XM_CALLCONV Draw(PrimitiveBatch<VertexPositionColor>* batch,
	const BoundingFrustum& frustum,
	FXMVECTOR color)
{
	XMFLOAT3 corners[BoundingFrustum::CORNER_COUNT];
	frustum.GetCorners(corners);

	VertexPositionColor verts[24] = {};
	verts[0].position = corners[0];
	verts[1].position = corners[1];
	verts[2].position = corners[1];
	verts[3].position = corners[2];
	verts[4].position = corners[2];
	verts[5].position = corners[3];
	verts[6].position = corners[3];
	verts[7].position = corners[0];

	verts[8].position = corners[0];
	verts[9].position = corners[4];
	verts[10].position = corners[1];
	verts[11].position = corners[5];
	verts[12].position = corners[2];
	verts[13].position = corners[6];
	verts[14].position = corners[3];
	verts[15].position = corners[7];

	verts[16].position = corners[4];
	verts[17].position = corners[5];
	verts[18].position = corners[5];
	verts[19].position = corners[6];
	verts[20].position = corners[6];
	verts[21].position = corners[7];
	verts[22].position = corners[7];
	verts[23].position = corners[4];

	for (size_t j = 0; j < _countof(verts); ++j)
	{
		XMStoreFloat4(&verts[j].color, color);
	}

	batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST, verts, _countof(verts));
}

bool DebugDraw::IsGroupEnabled(std::string groupName)
{
	if (groups.find(groupName) == groups.end())
	{
		groups.insert(std::pair<std::string, bool>(groupName, true));
	}

	return groups[groupName];
}

void DebugDraw::Draw(ID3D11DeviceContext *context, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount)
{
	UINT stride = sizeof(VertexColor);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(indexCount, 0, 0);
}

void DebugDraw::SetWireframeDrawEnabled(bool enabled)
{
	isWireframeEnabled = enabled;
}

void DebugDraw::DrawAsWireframeIfEnabled()
{
	if (isWireframeEnabled)
	{
		core->GetDeviceContext()->RSSetState(wireframeRasterizer);
	}
}

bool DebugDraw::IsEnabled()
{
	return enabled;
}

void DebugDraw::SetEnabled(bool enable)
{
	enabled = enable;
}


DebugDraw * DebugDraw::GetInstance()
{
	return instance;
}

void DebugDraw::SetGroupActive(std::string group, bool active)
{
	if (groups.find(group) != groups.end())
	{
		groups[group] = active;
	}
}

void DebugDraw::Draw(PrimitiveShapesType shape, void* payload)
{
	DrawCallPayLoad call;
	call.shape = shape;
	call.payload = payload;
	drawCalls.push(call);
	//drawCalls.push({ shape, payload, transform });
}

void DebugDraw::Draw(Ray ray, std::string group)
{
	if (IsGroupEnabled(group))
		rays.push(ray);
}

void DebugDraw::Draw(Grid grid, std::string group)
{
	if (IsGroupEnabled(group))
		grids.push(grid);
}

void DebugDraw::Draw(Sphere shape, std::string group)
{
	if (IsGroupEnabled(group))
		spheres.push(shape);
}

void DebugDraw::Draw(Box shape, std::string group)
{
	if (IsGroupEnabled(group))
		boxes.push(shape);
}

void DebugDraw::Draw(Frustum shape, std::string group)
{
	if (IsGroupEnabled(group))
		frustums.push(shape);
}

void DebugDraw::Draw(Cylinder shape, std::string group)
{
	if (IsGroupEnabled(group))
	{
		cylinders.push(shape);
	}
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
	auto vs = rm->debugVertexShader;

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //Ensure only lines are drawn
	vs->SetMatrix4x4(WORLD_STR, Transform().GetWorldMatrix());
	vs->SetMatrix4x4(VIEW_STR, camera->GetViewMatrix());
	vs->SetMatrix4x4(PROJECTION_STR, camera->GetProjectionMatrix());
	vs->CopyAllBufferData();
	ps->CopyAllBufferData();
	ps->SetShader();
	vs->SetShader();
	batch->Begin();
	while (!drawCalls.empty())
	{
		auto call = drawCalls.front();
		drawCalls.pop();

		switch (call.shape)
		{
		case RAY:
		{
			Ray* ray = reinterpret_cast<Ray*>(call.payload);
			DrawRay(batch.get(), XMLoadFloat3(&ray->origin), XMLoadFloat3(&ray->direction) * ray->length, false, XMLoadFloat4(&ray->color));
			break;
		}
		case GRID:
		{
			auto grid = reinterpret_cast<Grid*>(call.payload);
			DrawGrid(batch.get(), XMLoadFloat3(&grid->xAxis), XMLoadFloat3(&grid->yAxis), XMLoadFloat3(&grid->origin), grid->xDivs, grid->yDivs, XMLoadFloat4(&grid->color));
			break;
		}
		case BOX:
		{
			auto shape = reinterpret_cast<Box*>(call.payload);
			::Draw(batch.get(), shape->bounding, XMLoadFloat4(&shape->color));
			break;
		}
		case SPHERE:
		{
			auto shape = reinterpret_cast<Sphere*>(call.payload);
			::Draw(batch.get(), shape->bounding, XMLoadFloat4(&shape->color));
			break;
		}
		case FRUSTUM:
		{
			auto shape = reinterpret_cast<Frustum*>(call.payload);
			::Draw(batch.get(), shape->bounding, XMLoadFloat4(&shape->color));
			break;
		}
		}
	}

	while (!rays.empty())
	{
		auto ray = rays.front();
		rays.pop();
		DrawRay(batch.get(), XMLoadFloat3(&ray.origin), XMLoadFloat3(&ray.direction) * ray.length, false, XMLoadFloat4(&ray.color));
	}

	while (!grids.empty())
	{
		auto grid = grids.front();
		grids.pop();
		DrawGrid(batch.get(), XMLoadFloat3(&grid.xAxis), XMLoadFloat3(&grid.yAxis), XMLoadFloat3(&grid.origin), grid.xDivs, grid.yDivs, XMLoadFloat4(&grid.color));
	}

	while (!boxes.empty())
	{
		auto shape = boxes.front();
		boxes.pop();
		::Draw(batch.get(), shape.bounding, XMLoadFloat4(&shape.color));
	}

	while (!spheres.empty())
	{
		auto shape = spheres.front();
		spheres.pop();
		::Draw(batch.get(), shape.bounding, XMLoadFloat4(&shape.color));
	}

	while (!frustums.empty())
	{
		auto shape = frustums.front();
		frustums.pop();
		::Draw(batch.get(), shape.bounding, XMLoadFloat4(&shape.color));
	}

	while (!cylinders.empty())
	{
		auto shape = cylinders.front();
		cylinders.pop();
		::Draw(batch.get(), shape);
	}

	//DrawGrid(batch.get(), XMVectorSet(100, 0, 0, 0), XMVectorSet(0, 0, 100, 0), XMVectorSet(0, -3, 0, 0), 100, 100, Colors::Green);
	batch->End();

	//Reset Pipeline 
	context->OMSetBlendState(0, 0, 0xFFFFFFFF);
	context->OMSetDepthStencilState(0, 0);
	context->RSSetState(nullptr);
}

DebugDraw::DebugDraw(SystemCore* core)
{
	this->core = core;
	auto device = core->GetDevice();
	batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(core->GetDeviceContext());
	states = std::make_unique<CommonStates>(core->GetDevice());
	instance = this;
	enabled = true;
	isWireframeEnabled = false;
	D3D11_RASTERIZER_DESC wfRasDesc = {};
	wfRasDesc.CullMode = D3D11_CULL_NONE;
	wfRasDesc.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&wfRasDesc, &wireframeRasterizer);
}


DebugDraw::~DebugDraw()
{
	if (wireframeRasterizer)
	{
		wireframeRasterizer->Release();
	}
}
