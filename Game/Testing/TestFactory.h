#pragma once
#include "EntityFactory.h"
#include "TestComponent.h"

class TestFactory final : public cecsar::EntityFactory<TestComponent, TestComponent>
{
protected:
	void OnConstruction(int32_t index, TestComponent&, TestComponent&) override;
};

inline void TestFactory::OnConstruction(const int32_t index, TestComponent&, TestComponent&)
{

}
