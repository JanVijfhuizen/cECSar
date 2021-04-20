#pragma once

#include "Cecsar.h"
#include "SoASet.h"
#include "MapSet.h"
#include <emmintrin.h>

namespace jecs::example
{
	struct Position
	{
		float x, y, z;
	};

	// To use the SoA set, inherit from it and define the member variable types 
	// via the template arguments.
	class PositionSoA : public jecs::SoASet<PositionSoA, float, float, float>
	{
	public:
		// This is an easy way to define the names of the member variables,
		// but is not required.
		enum
		{
			x, y, z
		};
	};

	template <typename T>
	class SomeDataStructure : public jecs::Set<SomeDataStructure<T>>
	{
	public:
		// Besides defining this method, you're completely free to define your own
		// data structure.
		void EraseAt(int32_t index) override
		{
			// Some behaviour.
		}
	};

	int RunExampleCustomSets()
	{
		jecs::Cecsar cecsar;
		jecs::Entity entity = cecsar.Spawn();

		// While the sparse set is good enough for most usecases, sometimes you'd want to define
		// your own data structure. You can do this by just inheriting from the Set class.
		SomeDataStructure<Position>& customSet = SomeDataStructure<Position>::Get();
		customSet.EraseAt(1);

		// Cecsar comes with a few data structures, like the map set.
		// While the sparse set is great, it requires a lot of memory, even when the set is barely filled.
		// The map set saves a lot of memory, and is useful for components that aren't common.
		auto& mapSet = jecs::MapSet<Position>::Get();

		// The map works very similar to the sparse set.
		mapSet.Insert(entity.index);
		for (auto [instance, index] : mapSet)
		{
			// Do some stuff.
			instance.x++;
		}

		// If you want to really go for performance, cecsar also comes with a SoA (Struct of Arrays) set.
		// Do be warned though, this is a bit weird to use.
		auto& soASet = PositionSoA::Get();

		// Get one of the vectorized member variable arrays.
		float* xArr = soASet.At<PositionSoA::x>();
		float* yArr = soASet.At<PositionSoA::y>();

		// Some example code. If the Y axis is higher than 5, we'll reset the X axis to 0.
		for (auto [dense, sparse] : soASet)
		{
			if (yArr[dense] > 5)
				xArr[dense] = 0;
		}

		// The SoA set is packed by default, so you can actually vectorize your code.
		// We're just going to multiply the x axes now, just to show how it can be done.
		__m128 mul4 = _mm_set_ps1(2);
		for (int i = 0; i < soASet.GetCount(); i += 4)
		{
			float* f = &xArr[i];
			__m128 f4 = _mm_load_ps(f);

			f4 = _mm_mul_ps(f4, mul4);
			_mm_store_ps(f, f4);
		}

		return 0;
	}
}