#include "stdafx.h"
#include "CppUnitTest.h"
#include "../FakeIt/single_header/mstest/fakeit.hpp"

#include "../Core.Game/Game.h"
#include "../Engine/Engine.h"
#include <exception>

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
			Assert::IsTrue(Game::GetInstanceCount()>0);
			delete gameInstance;
		}

		TEST_METHOD(Game_SetSpeed)
		{
			Game *gameInstance = Game::CreateInstance();
			float speed = -2.f;
			gameInstance->SetSpeed(speed);
			float actual = gameInstance->GetSpeed();
			Assert::AreEqual(speed, actual);
		}

		TEST_METHOD(Game_SendInput_CheckSpeed)
		{
			Game *gameInstance = Game::CreateInstance();
			GameEntity *entity = new GameEntity();		
			float speed = 2.f;
			entity->SetPosition(Vector3f(0.f, 0.f, 0.f));
			gameInstance->SetSpeed(speed);
			std::string entityName = "MainEntity";
			
			gameInstance->AddEntity(entity, entityName);
			gameInstance->SendInput(Up, entityName);
			
			auto position = entity->GetPosition();
			auto expected = Vector3f(0.f, speed, 0.f);
			Assert::AreEqual(expected.x, position.x);
			Assert::AreEqual(expected.y, position.y);
			Assert::AreEqual(expected.z, position.z);
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

		TEST_METHOD(Game_SendInput_Down)
		{
			Game *gameInstance = Game::CreateInstance();
			GameEntity *entity = new GameEntity();
			Mock<GameEntity> gameEntityMock(*entity);
			std::string entityName = "MainEntity";
			Fake(Method(gameEntityMock, MoveDown));
			gameInstance->AddEntity(entity, entityName);
			gameInstance->SendInput(Down, entityName);
			Verify(Method(gameEntityMock, MoveDown));
			delete gameInstance;
		}

		TEST_METHOD(Game_SendInput_Left)
		{
			Game *gameInstance = Game::CreateInstance();
			GameEntity *entity = new GameEntity();
			Mock<GameEntity> gameEntityMock(*entity);
			std::string entityName = "MainEntity";
			Fake(Method(gameEntityMock, MoveLeft));
			gameInstance->AddEntity(entity, entityName);
			gameInstance->SendInput(Left, entityName);
			Verify(Method(gameEntityMock, MoveLeft));
			delete gameInstance;
		}

		TEST_METHOD(Game_SendInput_Right)
		{
			Game *gameInstance = Game::CreateInstance();
			GameEntity *entity = new GameEntity();
			Mock<GameEntity> gameEntityMock(*entity);
			std::string entityName = "MainEntity";
			Fake(Method(gameEntityMock, MoveRight));
			gameInstance->AddEntity(entity, entityName);
			gameInstance->SendInput(Right, entityName);
			Verify(Method(gameEntityMock, MoveRight));
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

		TEST_METHOD(Game_Update)
		{
			Game *game = Game::CreateInstance();
			Keyboard keyboard;
			Mock<Game> gameMock(*game);
			Fake(Method(gameMock, SendInput));
			game->BindKeyboard(&keyboard);
			Mock<Keyboard> kbMock(keyboard);
			When(Method(kbMock, IsKeyPressed)).Return(true);
			game->Update();
			Verify(Method(gameMock, SendInput));
		}

		TEST_METHOD(Game_ReturnAllEntities)
		{
			Game *game = Game::CreateInstance();
			game->AddEntity(new GameEntity(), "A");
			game->AddEntity(new GameEntity(), "B");
			std::vector<Entity*> entities = game->GetEntities();
			Assert::AreEqual((size_t)2, entities.size());
		}

	};
}