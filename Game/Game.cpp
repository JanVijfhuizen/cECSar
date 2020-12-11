#include "Cecsar.h"

#include "Testing/TestSystem.h"
#include "Testing/TestModule.h"
#include "Testing/TestFactory.h"

int main()
{
	cecsar::Cecsar cecsar{};
	cecsar.Update<TestSystem>();
	auto& module = cecsar.GetModule<TestModule>();

	cecsar.AddEntity<TestFactory>();

	auto& component = cecsar.AddComponent<TestBlockA>(12);
	cecsar.RemoveComponent<TestBlockB>(7);

	cecsar.RemoveEntity(0);

	return 0;
}
