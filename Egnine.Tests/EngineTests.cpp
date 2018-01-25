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
			CoreGame *game = new CoreGame(1280, 720, "DXGame");
			HINSTANCE hInstance = 0; //Fake HINSTANCE		
			SystemCore &core = *game->GetSystemCore();
			Mock<SystemCore> coreMock(core);
			Fake(Method(coreMock, InitializeWindow));
			Fake(Method(coreMock, InitializeAndBindDirectX));
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
			Mock<CoreGame> gameMock(*game);
			Mock<IGame> gameInstanceMock;
			When(Method(gameInstanceMock, Update)).Do([]() {
				game->SetState(Quit);
			});
			game->Bind(&gameInstanceMock.get());
			game->Run();
			Assert::AreEqual(Quit, game->GetState());
		}

		TEST_METHOD_CLEANUP(Test_Cleanup)
		{
			delete game;
		}

	};
}