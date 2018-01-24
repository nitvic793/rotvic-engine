#include "stdafx.h"
#include "CppUnitTest.h"
#include "../FakeIt/single_header/mstest/fakeit.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace GameCoreTests
{		
	TEST_CLASS(GameTests)
	{
	public:

		TEST_METHOD(GameEntity_Initialize)
		{
			GameEntity gameEntity;
			Vector3f position = gameEntity.GetPosition();
			Vector3f expected(0.f, 0.f, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}
		
		TEST_METHOD(GameEntity_Move)
		{
			
		}

		TEST_METHOD(GameEntity_MoveUp)
		{

		}

		TEST_METHOD(GameEntity_MoveDown)
		{

		}

		TEST_METHOD(GameEntity_MoveLeft)
		{

		}

		TEST_METHOD(GameEntity_MoveRight)
		{

		}

	};
}