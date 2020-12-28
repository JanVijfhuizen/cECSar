#pragma once
#include <pch.h>
#include <cstdint>

namespace cecsar
{
	struct EntityInfo final
	{
		int32_t index = -1;
		int32_t globalId = -1;

		inline EntityInfo();
		inline EntityInfo(int32_t index, int32_t globalId);

		constexpr bool operator==(const EntityInfo& other) const;
	};

	constexpr bool EntityInfo::operator==(const EntityInfo& other) const
	{
		return index == other.index && globalId == other.globalId;
	}

	inline EntityInfo::EntityInfo() = default;

	inline EntityInfo::EntityInfo(const int32_t index, const int32_t globalId) :
		index(index), globalId(globalId)
	{

	}
}
