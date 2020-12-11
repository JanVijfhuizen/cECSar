#pragma once
#include <ComponentSystem.h>

struct TestBlockA
{
	
};

struct TestBlockB
{
	
};

class TestSystem : public cecsar::ComponentSystem<TestBlockA, TestBlockB>
{
private:
	void OnUpdate(utils::SparseSet<TestBlockA>&, utils::SparseSet<TestBlockB>&) override;
};
