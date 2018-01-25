#include "stdafx.h"
#include "CppUnitTest.h"
#include "../Engine/CoreGame.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace EngineTests
{		
	TEST_CLASS(CoreGameTests)
	{
	public:
		
		TEST_METHOD(CoreGame_Initialization)
		{
			CoreGame *game = new CoreGame(1280, 720, "DXGame");
			Assert::IsTrue(game->InitializeWindow());
		}

	};
}