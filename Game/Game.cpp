//#include <Engine.h>
#include <CecsarRevamped.h>
#include <iostream>

struct SomeComponent final
{
	int32_t x, y;
};

struct SomeOtherComponent final
{
	int32_t a, b, c;
};

class SomeFactory final : public revamped::Cecsar::Factory
{
private:
	void OnConstruct(revamped::Cecsar& cecsar, int32_t id) override;
};

void SomeFactory::OnConstruct(revamped::Cecsar& cecsar, const int32_t id)
{
	auto& component = cecsar.GetSet<SomeComponent>().Insert(id);
	component.x = 500;
}

class SomeSystem final : public revamped::Cecsar::System<SomeComponent, SomeOtherComponent>
{
	void OnUpdate(utils::SparseSet<SomeComponent>& instances, utils::SparseSet<SomeOtherComponent>&) override;
};

void SomeSystem::OnUpdate(utils::SparseSet<SomeComponent>& instances, utils::SparseSet<SomeOtherComponent>&)
{
	for (auto& instance : instances)
		instance.x--;
}

int main(int argc, char* argv[])
{
	//game::Engine::Run();
	revamped::Cecsar cecsar{{static_cast<int32_t>(1e5)}};

	auto& factory = cecsar.Get<SomeFactory>();

	for (int i = 0; i < 1e4; ++i) {
		const auto entity = cecsar.Spawn();
		factory.Construct(entity.index);
	}

	while(true)
		cecsar.Get<SomeSystem>().Update();
	
	return 0;
}
