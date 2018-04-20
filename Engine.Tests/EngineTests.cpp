#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Engine/Engine.h"
#include "../Engine/SystemCore.h"

#include "../FakeIt/single_header/mstest/fakeit.hpp"

using namespace fakeit;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{
	class IGameMock : public IGame
	{
		// Inherited via IGame
		virtual void Update(float) override
		{
		}
		virtual void Initialize() override
		{
		}

		std::vector<Entity*> GetEntities() override
		{
			return std::vector<Entity*>();
		}
	};

	TEST_CLASS(CoreGameTests)
	{
		CoreGame *game;

	public:

		TEST_METHOD_INITIALIZE(Test_Initialization)
		{
			game = new CoreGame(720, 1280, "DXGame");
			HINSTANCE hInstance = 0; //Fake HINSTANCE		
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			ResourceManager &rm = *game->GetResourceManager();			
			Mock<ResourceManager> rmMock(rm);
			Fake(Method(rmMock, LoadResources));
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, CreateConsoleWindow));
			Fake(Method(coreMock, Run));
			game->Initialize(hInstance, 0);
		}

		TEST_METHOD(CoreGame_Initialization)
		{
			CoreGame *game = new CoreGame(720, 1280, "DXGame");
			HINSTANCE hInstance = 0; //Fake HINSTANCE		
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			ResourceManager &rm = *game->GetResourceManager();
			Mock<ResourceManager> rmMock(rm);
			Fake(Method(rmMock, LoadResources));
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, CreateConsoleWindow));
			Assert::IsTrue(game->Initialize(hInstance, 0));
			Verify(Method(coreMock, InitializeWindow));
			Verify(Method(coreMock, InitializeAndBindDirectX));
			delete game;
		}

		TEST_METHOD(CoreGame_Initialization_InvalidAspectRatio)
		{
			CoreGame *game = new CoreGame(725, 1280, "DXGame");
			HINSTANCE hInstance = 0; //Fake HINSTANCE		
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			ResourceManager &rm = *game->GetResourceManager();
			Mock<ResourceManager> rmMock(rm);
			Fake(Method(rmMock, LoadResources));
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Assert::ExpectException<std::exception>([&]() {
				game->Initialize(hInstance, 0);
			});
			delete game;
		}

		TEST_METHOD(CoreGame_Run)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Mock<Renderer> renderMock(*game->GetRenderer());
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, Run));
			Fake(Method(renderMock, Initialize));

			IGame *iGameInstance = new IGameMock();
			Mock<IGame> gameInstanceMock(*iGameInstance);
			When(Method(gameInstanceMock, Update))
				.Do([&](float deltaTime) {
				game->SetState(Quit);
			});
			game->Bind(iGameInstance);
			game->Run();
			Verify(Method(coreMock, Run));
			Verify(Method(renderMock, Initialize));
		}

		TEST_METHOD(CoreGame_Run_Exception)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Mock<Renderer> renderMock(*game->GetRenderer());
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, HandleError));
			When(Method(coreMock, Run)).Throw(std::exception("Test error"));
			Fake(Method(renderMock, Initialize));
			IGame *iGameInstance = new IGameMock();
			Mock<IGame> gameInstanceMock(*iGameInstance);
			game->Bind(iGameInstance);
			game->Run();
			Verify(Method(renderMock, Initialize));
			Verify(Method(coreMock, HandleError));
		}

		TEST_METHOD(CoreGame_ClearScreen)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, Run));
			Fake(Method(coreMock, ClearScreen));
			game->ClearScreen();
			Verify(Method(coreMock, ClearScreen));
		}

		TEST_METHOD(CoreGame_BindNullInstance)
		{
			CoreGame *game = new CoreGame(720, 1280, "DXGame");
			Assert::ExpectException<std::exception>([&]() {
				game->Bind(nullptr);
			});
			delete game;
		}

		TEST_METHOD(Renderer_DrawMesh)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, Draw));
			Mesh *mesh = new Mesh(game->GetSystemCore());
			Renderer *renderer = new Renderer(game->GetSystemCore());
			Mock<SystemRenderer> sysRendMock(*renderer->GetInternalRenderer());
			Fake(Method(sysRendMock, Draw));
			Fake(Method(sysRendMock, SetShaders));
			renderer->Draw(mesh);
			Verify(Method(coreMock, Draw));
			Verify(Method(sysRendMock, Draw));
		}

		TEST_METHOD(Renderer_DrawMesh_Null)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, Draw));
			Mesh *mesh = nullptr;
			Renderer *renderer = new Renderer(game->GetSystemCore());
			Assert::ExpectException<std::exception>([&]() {
				renderer->Draw(mesh);
			});
		}

		TEST_METHOD(Renderer_DrawEntity)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, Draw));
			Entity *entity = new Entity(nullptr);
			Mesh *m = new Mesh(game->GetSystemCore());
			entity->SetMesh(m);
			Renderer *renderer = new Renderer(game->GetSystemCore());
			Mock<SystemRenderer> sysRendMock(*renderer->GetInternalRenderer());
			Fake(Method(sysRendMock, Draw));
			Fake(Method(sysRendMock, SetShaders));
			renderer->Draw(entity);
			Verify(Method(coreMock, Draw));
			Verify(Method(sysRendMock, Draw));
		}

		TEST_METHOD(Renderer_DrawEntity_Null)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, Draw));
			Entity *entity = nullptr;
			Renderer *renderer = new Renderer(game->GetSystemCore());
			Assert::ExpectException<std::exception>([&]() {
				renderer->Draw(entity);
			});
		}

		TEST_METHOD(Entity_SetMesh)
		{
			Entity *entity = new Entity(nullptr);
			Mesh *expected = new Mesh(game->GetSystemCore());
			entity->SetMesh(expected);
			auto actual = entity->GetMesh();
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD(Mesh_SetCore_Null)
		{
			Assert::ExpectException<std::exception>([&]() {
				Mesh *mesh = new Mesh(nullptr);
			});
		}

		TEST_METHOD(SaveSystem_Save)
		{
			FileBuffer *buffer = new FileBuffer();
			Mock<FileBuffer> mockBuffer(*buffer);
			Mock<ISerializable> mockSerialize;
			SaveSystem sys(buffer);
			Fake(Method(mockBuffer, Save));
			Fake(Method(mockSerialize, Serialize));
			sys.Save(&mockSerialize.get(), "Test");
			Verify(Method(mockBuffer, Save));
			Verify(Method(mockSerialize, Serialize));
		}

		TEST_METHOD(SaveSystem_NullBuffer)
		{
			Assert::ExpectException<std::invalid_argument>([&]() 
			{
				SaveSystem sys(nullptr); 
			});
		}

		TEST_METHOD(SaveSystem_Load)
		{
			FileBuffer *buffer = new FileBuffer();
			Mock<FileBuffer> mockBuffer(*buffer);
			Mock<ISerializable> mockSerialize;
			SaveSystem sys(buffer);
			Fake(Method(mockBuffer, Load));
			//Fake(Method(mockBuffer, GetBuffer));
			Fake(Method(mockSerialize, Deserialize));
			sys.Load(&mockSerialize.get(), "Test");
			Verify(Method(mockBuffer, Load));
			Verify(Method(mockSerialize, Deserialize));
		}

		TEST_METHOD_CLEANUP(Test_Cleanup)
		{
			delete game;
		}

	};
}