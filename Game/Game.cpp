#include "Cecsar.h"

#include "Testing/TestSystem.h"
#include "Testing/TestModule.h"
#include "Testing/TestFactory.h"

#include "SDL.h"

int main(int argc, char* argv[])
{
	SDL_Init(0);

	cecsar::Cecsar cecsar{};
	cecsar.Update<TestSystem>();
	auto& module = cecsar.GetModule<TestModule>();

	delete [] cecsar.AddEntity<TestFactory>(5);
	delete [] cecsar.AddEntity<>();

	auto& component = cecsar.AddComponent<TestBlockA>(12);
	cecsar.RemoveComponent<TestBlockB>(7);

	cecsar.RemoveEntity(0);

	SDL_Quit();
	return 0;
}
