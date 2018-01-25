#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Engine/Engine.h"
#include "../Engine/SystemCore.h"
#include "../Engine/IGame.h"
#include "../FakeIt/single_header/mstest/fakeit.hpp"

using namespace fakeit;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{
	TEST_CLASS(CoreGameTests)
	{
		CoreGame *game;

	public:

		TEST_METHOD_INITIALIZE(Test_Initialization)
		{
			game = new CoreGame(1280, 720, "DXGame");
			HINSTANCE hInstance = 0; //Fake HINSTANCE		
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, Run));
			game->Initialize(hInstance, 0);
		}

		TEST_METHOD(CoreGame_Initialization)
		{
			CoreGame *game = new CoreGame(1280, 720, "DXGame");
			HINSTANCE hInstance = 0; //Fake HINSTANCE		
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Assert::IsTrue(game->Initialize(hInstance, 0));
			Verify(Method(coreMock, InitializeWindow));
			Verify(Method(coreMock, InitializeAndBindDirectX));
			delete game;
		}

		TEST_METHOD(CoreGame_Run)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
			Fake(Method(coreMock, Run));
			class IGameMock : public IGame { public: virtual void Update() {} std::vector<Entity*> GetEntities() { return std::vector<Entity*>(); } };
			IGame *iGameInstance = new IGameMock();
			Mock<IGame> gameInstanceMock(*iGameInstance);
			When(Method(gameInstanceMock, Update))
				.Do([&]() {
				game->SetState(Quit);
			});
			game->Bind(iGameInstance);
			game->Run();
			Verify(Method(coreMock, Run));
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
			CoreGame *game = new CoreGame(1280, 720, "DXGame");
			Assert::ExpectException<std::exception>([&]() {
				game->Bind(nullptr);
			});
			delete game;
		}

		TEST_METHOD(Entity_SetPosition)
		{
			Entity entity;
			auto expected = Vector3f(1.f, 1.f, 1.f);
			entity.SetPosition(expected);
			auto position = entity.GetPosition();
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}

		TEST_METHOD(Renderer_DrawMesh)
		{
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, Draw));
			Mesh *mesh = new Mesh(game->GetSystemCore());
			Renderer *renderer = new Renderer(game->GetSystemCore());
			renderer->Draw(mesh);
			Verify(Method(coreMock, Draw));
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
			Entity *entity= new Entity();
			Mesh *m = new Mesh(game->GetSystemCore());
			entity->SetMesh(m);
			Renderer *renderer = new Renderer(game->GetSystemCore());
			renderer->Draw(entity);
			Verify(Method(coreMock, Draw));
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
			Entity *entity = new Entity();
			Mesh *expected = new Mesh(game->GetSystemCore());
			entity->SetMesh(expected);
			auto actual = entity->GetMesh();
			Assert::IsTrue(expected == actual);
		}

		TEST_METHOD_CLEANUP(Test_Cleanup)
		{
			delete game;
		}

	};
}