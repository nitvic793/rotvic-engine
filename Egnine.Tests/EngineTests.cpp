#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Engine/CoreGame.h"
#include "../Engine/SystemCore.h"
#include "../FakeIt/single_header/mstest/fakeit.hpp"

using namespace fakeit;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{		
	TEST_CLASS(CoreGameTests)
	{
	public:
		
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

	};
}