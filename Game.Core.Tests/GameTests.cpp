#include "stdafx.h"
#include "CppUnitTest.h"
#include "../FakeIt/single_header/mstest/fakeit.hpp"

#include "../Core.Game/Game.h"
#include "../Engine/Engine.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace fakeit;

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

		TEST_METHOD(Game_Initialize)
		{
			Game *gameInstance = Game::CreateInstance();
			Assert::IsNotNull(gameInstance);
			Assert::AreEqual(1, Game::GetInstanceCount());
			delete gameInstance;
		}

		TEST_METHOD(Game_SendInput_Up)
		{
			Game *gameInstance = Game::CreateInstance();	
			GameEntity *entity = new GameEntity();
			Mock<GameEntity> gameEntityMock(*entity);
			std::string entityName = "MainEntity";
			Fake(Method(gameEntityMock, MoveUp));
			gameInstance->AddEntity(entity, entityName);
			gameInstance->SendInput(Up, entityName);
			Verify(Method(gameEntityMock, MoveUp));
			delete gameInstance;
		}
		
		TEST_METHOD(GameEntity_Move)
		{
			GameEntity gameEntity;			
			gameEntity.Move(Vector3f(1.f, 0.f, 0.f));
			Vector3f position = gameEntity.GetPosition();
			Vector3f expected(1.f, 0.f, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}

		TEST_METHOD(GameEntity_MoveUp)
		{
			GameEntity gameEntity;
			float speed = 1.f;
			gameEntity.MoveUp(speed);
			Vector3f position = gameEntity.GetPosition();
			Vector3f expected(0.f, speed, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}

		TEST_METHOD(GameEntity_MoveDown)
		{
			GameEntity gameEntity;
			float speed = 1.f;
			gameEntity.MoveDown(speed);
			Vector3f position = gameEntity.GetPosition();
			Vector3f expected(0.f, -speed, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}

		TEST_METHOD(GameEntity_MoveLeft)
		{
			GameEntity gameEntity;
			float speed = 1.f;
			gameEntity.MoveLeft(speed);
			Vector3f position = gameEntity.GetPosition();
			Vector3f expected(-speed, 0.f, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}

		TEST_METHOD(GameEntity_MoveRight)
		{
			GameEntity gameEntity;
			float speed = 1.f;
			gameEntity.MoveRight(speed);
			Vector3f position = gameEntity.GetPosition();
			Vector3f expected(speed, 0.f, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
		}

	};
}