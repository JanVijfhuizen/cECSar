#include "pch.h"
#include "Entity.h"

namespace jecs
{
	size_t Entity::Hasher::operator()(const Entity& entity) const
	{
		return entity.index;
	}

	Entity::Entity(const int32_t index, const int32_t id) : index(index), id(id)
	{

	}

	bool Entity::operator==(const Entity& other) const
	{
		return id == other.id;
	}

	bool Entity::operator!=(const Entity& other) const
	{
		return !(*this == other);
	}
}