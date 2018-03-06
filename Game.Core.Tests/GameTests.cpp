#include "stdafx.h"
#include "CppUnitTest.h"
#include "../FakeIt/single_header/mstest/fakeit.hpp"


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

		TEST_METHOD(GameEntity_New_NotNull)
		{
			GameEntity *entity = nullptr;
			entity = new GameEntity();
			Assert::IsNotNull(entity);
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
			Mouse mouse(nullptr);
			Mock<Game> gameMock(*game);
			game->BindKeyboard(&keyboard);
			game->BindMouse(&mouse);
			game->AddEntity(new GameEntity(), "Test");
			Mock<Keyboard> kbMock(keyboard);
			Mock<Mouse> mouseMock(mouse);
			When(Method(mouseMock, IsKeyPressed)).AlwaysReturn(false);
			game->Update(1.f);
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