#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Engine/CoreGame.h"
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

		TEST_METHOD_CLEANUP(Test_Cleanup)
		{
			delete game;
		}

	};
}