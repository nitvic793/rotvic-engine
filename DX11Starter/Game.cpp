#include "Game.h"



using namespace DirectX;


Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexBuffer = 0;
	indexBuffer = 0;
	vertexShader = 0;
	pixelShader = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	//printf("Console window created successfully.  Feel free to printf() here.");
#endif
	
}


Game::~Game()
{
	// Release any (and all!) DirectX objects
	// we've made in the Game class
	

	//delete gobj1;
	//delete mesh1;
	
	//delete mesh2;

	srv->Release();
	sampler->Release();
	
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }


}


void Game::Init()
{
	LoadShaders();

	LoadTexture(device, context, L"BrickTexture.jpg", 0, &srv);

	D3D11_SAMPLER_DESC sd = {}; // Zeros it out
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // Tri-linear filtering
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&sd, &sampler);

	CreateMatrices();
	CreateBasicGeometry();
	CreateGameObjects();
	

	light1.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	light1.DiffuseColor = XMFLOAT4(0, 0.5f, 1, 1);
	light1.Direction = XMFLOAT3(1,-1,0);

	light2.AmbientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 0.1f);
	light2.DiffuseColor = XMFLOAT4(0.8f, 0.5f, 0, 1);
	light2.Direction = XMFLOAT3(-1, 0, 0);
	
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	printf("Use WASD to move forward and strafe camera. Hold left mouse button and drag to rotate.\nSpace and X raise and lower the camera respectively");
	
	
}


void Game::LoadShaders()
{
	std::shared_ptr<SimpleVertexShader> VS(new SimpleVertexShader(device, context));
	vertexShader = VS;
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	std::shared_ptr<SimplePixelShader> PS(new SimplePixelShader(device, context));
	pixelShader = PS;
	pixelShader->LoadShaderFile(L"PixelShader.cso");
	
}

void Game::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext,const wchar_t* fileName, ID3D11Resource** resource, ID3D11ShaderResourceView** srv)
{


	CreateWICTextureFromFile(device, deviceContext, fileName, resource, srv);
}



void Game::CreateMatrices()
{

	camera.update();
	viewMatrix = camera.getViewMatrix();
	
	camera.updateProjectionMatrix((float)width / height);
	projectionMatrix = camera.getProjectionMatrix();

}



void Game::CreateBasicGeometry()
{

	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	/*
	Vertex vertices1[] =
	{
	{ XMFLOAT3(-0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	{ XMFLOAT3(+0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	{ XMFLOAT3(+0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	{ XMFLOAT3(-0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};
	*/
	


	unsigned int indices1[] = { 0, 1, 2, 0, 2, 3};

	
	//std::shared_ptr<Mesh> M1(new Mesh(device,1));
	//mesh1 = M1;
	//Taking different obj files
	
	//std::shared_ptr<Mesh> M2(new Mesh("sphere.obj", device));
	
	fbxLoader.LoadNodes(fbxLoader.scene->GetRootNode(), device);
	FbxNode* childNode = fbxLoader.scene->GetRootNode()->GetChild(0);
	FbxString name1 = childNode->GetName();
	mesh2 = fbxLoader.GetMesh(childNode,device);
	
	/*
	std::shared_ptr<Mesh> M3(new Mesh("helix.obj", device));
	mesh3 = M3;
	*/
	
	//Initializing material here
	std::shared_ptr<Material> T1 (new Material(vertexShader,pixelShader,srv,sampler));
	material1 = T1;


}

void Game::CreateGameObjects()
{
	std::shared_ptr<GameEntity> G1 (new GameEntity(mesh2, material1));
	gobj1 = G1;
	std::shared_ptr<GameEntity> G2(new GameEntity(mesh2, material1));
	gobj2 = G2;
	std::shared_ptr<GameEntity> G3(new GameEntity(mesh2, material1));
	gobj3 = G3;

	//FbxAnimEvaluator* lEvaluator = fbxLoader.scene->GetAnimationEvaluator();

	gobj1->setPosition(0, 0, 10);
	gobj2->setPosition(15, 0, 10);
	gobj3->setPosition(-15, 0, 10);
	//gobj1->setRotation(-1.57,0,0);
	//gobj2->setRotation(-1.57, 0, 0);
	//gobj3->setRotation(-1.57, 0, 0);

	
}


void Game::OnResize()
{
	DXCore::OnResize();

	camera.updateProjectionMatrix((float)width / height);
	projectionMatrix = camera.getProjectionMatrix();
}


void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
	else if (GetAsyncKeyState('A') & 0x8000 )
	{
		camera.strafeCamera(5, deltaTime);
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		camera.strafeCamera(-5, deltaTime);
	}
	else if (GetAsyncKeyState('W') & 0x8000)
	{
		camera.moveCameraForward(5,deltaTime);
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		camera.moveCameraForward(-5, deltaTime);
	}
	else if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		camera.raiseCamera(5, deltaTime);
	}
	else if (GetAsyncKeyState('X') & 0x8000)
	{
		camera.raiseCamera(-5, deltaTime);
	}
	
	
	viewMatrix = camera.getViewMatrix();


	//gobj1->setRotation(0,0,5);
	//gobj1->animate();
	
	//gobj2->setScale((float)(sin(i*0.001) + 1.25),(float)(sin(i*0.001) + 1.25), 0);
	//gobj3->setScale((float)(sin(i*0.001) + 1.25),(float)(sin(i*0.001) + 1.25), 0);
	//gobj4->setPosition(2.5, (float)sin(i*0.001),0);
	//gobj5->setPosition(-2.5, (float)sin(i*0.001), 0);

}


void Game::Draw(float deltaTime, float totalTime)
{	
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	
	//XMFLOAT4X4 animatedMatrix = gobj1->GetAnimatedMatrix();

	int numBones = fbxLoader.skeleton.mJoints.size();

	fbxLoader.GetAnimatedMatrixExtra();

	for (int i = 0; i < numBones; i++)
	{
		//bones[i].BoneTransform = (fbxLoader.skeleton.mJoints[i].mTransform);

		//bones[i].BoneTransform = XMFLOAT4X4(1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

		//XMFLOAT4X4 jointTransform = (fbxLoader.skeleton.mJoints[i].mTransform);
		//XMFLOAT4X4 invJointTransform = (fbxLoader.skeleton.mJoints[i].mGlobalBindposeInverse);

		XMMATRIX jointTransformMatrix = XMLoadFloat4x4(&fbxLoader.skeleton.mJoints[i].mTransform);
		XMMATRIX invJointTransformMatrix = XMLoadFloat4x4(&fbxLoader.skeleton.mJoints[i].mGlobalBindposeInverse);

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


	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	pixelShader->SetData("light1", &light1, sizeof(DirectionalLight));
	pixelShader->SetData("light2", &light2, sizeof(DirectionalLight));
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	

	gobj1->prepareMaterial(viewMatrix,projectionMatrix);

	int bonesSize = 0;

	bonesSize = (sizeof(XMFLOAT4X4) * 72 *2);

	vertexShader->SetData("bones", &bones, bonesSize);

	ID3D11Buffer * v = gobj1->getMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &v, &stride, &offset);
	context->IASetIndexBuffer(gobj1->getMesh()->indexBufferPointer, DXGI_FORMAT_R32_UINT, 0);
    context->DrawIndexed(gobj1->getMesh()->GetIndexCount(), 0, 0);
	




	
	gobj2->prepareMaterial(viewMatrix, projectionMatrix);

	v = gobj2->getMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &v, &stride, &offset);
	context->IASetIndexBuffer(gobj2->getMesh()->indexBufferPointer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(gobj2->getMesh()->GetIndexCount(), 0, 0);
	

	


	
	gobj3->prepareMaterial(viewMatrix, projectionMatrix);

	v = gobj3->getMesh()->GetVertexBuffer();

	context->IASetVertexBuffers(0, 1, &v, &stride, &offset);
	context->IASetIndexBuffer(gobj3->getMesh()->indexBufferPointer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(gobj3->getMesh()->GetIndexCount(), 0, 0);




	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;


	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (buttonState & 0x0001)
	{
		camera.setRotation(camera.getXRotation() + (x - prevMousePos.x)*0.001f, camera.getYRotation()+ (y - prevMousePos.y)*0.001f);
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion