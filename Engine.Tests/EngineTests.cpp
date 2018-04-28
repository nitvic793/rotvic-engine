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

	public:

		TEST_METHOD_INITIALIZE(Test_Initialization)
		{
		}

		TEST_METHOD(Queue_Push)
		{
			ConcurrentQueue<int> queue;
			queue.Push(1);
			Assert::AreEqual(1, queue.Pop());
		}

		TEST_METHOD_CLEANUP(Test_Cleanup)
		{
			//delete game;
		}

	};
}