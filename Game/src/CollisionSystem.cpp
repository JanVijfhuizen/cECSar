﻿#include <Systems/CollisionSystem.h>
#include "Systems/TransformSystem.h"
#include "Modules/RenderModule.h"
#include <iostream>

game::CollisionSystem::~CollisionSystem()
{
	delete _quadTree;
	delete[] _transformBuffer;
}

void game::CollisionSystem::Initialize(cecsar::Cecsar& cecsar)
{
	JobSystem<Collider, Transform>::Initialize(cecsar);

	auto& renderModule = cecsar.GetModule<RenderModule>();

	const float w = renderModule.SCREEN_WIDTH;
	const float h = renderModule.SCREEN_HEIGHT;

	_transformSystem = &cecsar.GetSystem<TransformSystem>();
	_quadTree = new utils::QuadTree(w, h);
	_transformBuffer = new TransformBuffer[cecsar.info.setCapacity];
}

void game::CollisionSystem::OnUpdate(
	utils::SparseSet<Collider>& colliders, 
	utils::SparseSet<Transform>& transforms)
{
	UpdateBuffer(colliders, transforms);
	FillQuadTree(colliders);
	IterateQuadTree(colliders);
}

void game::CollisionSystem::UpdateBuffer(
	utils::SparseSet<Collider>& colliders, 
	utils::SparseSet<Transform>& transforms) const
{
	const auto dense = colliders.GetDenseRaw();
	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];
		auto& local = transforms.Get(index);

		auto& buffer = _transformBuffer[index];
		auto world = _transformSystem->ToWorld(local);

		const float delta = (world.position - buffer.world.position).SquaredLength();
		buffer.moved = delta > 1e-4;
		buffer.world = world;
	}
}

void game::CollisionSystem::FillQuadTree(utils::SparseSet<Collider>& colliders) const
{
	auto& jobModule = GetJobModule();

	// Remove the instances that are either invalid or have moved (too much).
	_quadTree->Iterate([this, &colliders, &jobModule](auto& nodes, const int32_t anchor)
		{
			for (int32_t i = nodes.size() - 1; i >= anchor; --i)
			{
				auto& node = *nodes[i];
				auto& quad = node.quad;
				auto& vector = node.instances;

				for (int32_t j = vector.size() - 1; j >= 0; --j)
				{
					const int32_t index = vector[j];
					auto& buffer = _transformBuffer[index];

					// Validate component.
					if (colliders.Contains(index))
					{
						// Check if moved.
						if (!buffer.moved)
							continue;

						// Despite being moved, check if it still part of this quad.
						auto& collider = colliders.Get(index);
						if (IntersectsQuad(collider, buffer.world, quad))
							continue;
					}

					// Remove entity.
					vector.erase(vector.begin() + j);
					buffer.sorted = false;
				}
			}

			return;

			// The node vector needs to be copied to be thread safe.
			jobModule.Enqueue([this, &colliders, nodes, anchor] 
				{
					
				});
		});

	// Implement this in other things as well.
	jobModule.Start();
	jobModule.Wait();

	// Clear the empty nodes.
	_quadTree->Clear(false);

	// Only push the instances that have changed their position within the tree.
	const auto dense = colliders.GetDenseRaw();
	for (int32_t i = colliders.GetCount() - 1; i >= 0; --i)
	{
		const int32_t index = dense[i];

		auto& collider = colliders[i];
		auto& buffer = _transformBuffer[index];
		if (buffer.sorted)
			continue;

		buffer.sorted = true;
		auto& world = buffer.world;

		// Push the colliders based on their positions.
		_quadTree->TryPush(index, [&collider, &world](
			const int32_t _, const utils::Quad& quad) 
			{
				return IntersectsQuad(collider, world, quad);
			});
	}
}

void game::CollisionSystem::IterateQuadTree(utils::SparseSet<Collider>& colliders) const
{
	std::vector<HitInfo> hits;

	// Check for possible collisions.
	_quadTree->Iterate([this, &colliders, &hits](
		auto& nodes, const int32_t anchor)
	{
		HitInfo aInfo{};
		HitInfo bInfo{};

		// List of nodes.
		for (int32_t i = nodes.size() - 1; i >= anchor; --i)
		{
			auto& node = *nodes[i];
			auto& vector = node.instances;

			// Check collision for own instances.
			for (int32_t j = vector.size() - 1; j >= 0; --j)
			{
				const int32_t index = vector[j];
				auto& collider = colliders.Get(index);
				auto& world = _transformBuffer[index].world;

				for (int32_t k = j - 1; k >= 0; --k)
				{
					const int32_t otherIndex = vector[j];
					auto& otherCollider = colliders.Get(otherIndex);
					auto& otherWorld = _transformBuffer[otherIndex].world;

					if (IntersectsOther(index, otherIndex, collider, world,
						otherCollider, otherWorld, aInfo, bInfo))
					{
						hits.push_back(aInfo);
						hits.push_back(bInfo);
					}
				}
			}

			// Check instance collisions against other nodes.
			for (int32_t j = i - 1; j >= 0; --j)
			{
				auto& otherNode = *nodes[j];
				auto& otherVector = otherNode.instances;

				for (auto& index : vector)
				{
					auto& collider = colliders.Get(index);
					auto& world = _transformBuffer[index].world;

					for (auto& otherIndex : otherVector)
					{
						auto& otherCollider = colliders.Get(otherIndex);
						auto& otherWorld = _transformBuffer[otherIndex].world;

						if (IntersectsOther(index, otherIndex, collider, world,
							otherCollider, otherWorld, aInfo, bInfo)) 
						{
							hits.push_back(aInfo);
							hits.push_back(bInfo);
						}
					}
				}
			}
		}
	});

	// Notify subscribers.
	for (auto& hit : hits)
		Notify(hit);
}

bool game::CollisionSystem::IntersectsQuad(const Collider& collider,
	const Transform& world, const utils::Quad& quad)
{
	const auto& position = world.position;

	const float xCol = position.x;
	const float yCol = position.y;

	const float xQuad = quad.pos.x;
	const float yQuad = quad.pos.y;

	const float quadWidth = quad.width;
	const float quadHeight = quad.height;

	const float radiusHalf = collider.radius / 2;

	// Horizontal check.
	if (xCol - radiusHalf < xQuad ||
		yCol - radiusHalf < yQuad)
		return false;

	// Vertical check.
	if (xCol + radiusHalf >= xQuad + quadWidth ||
		yCol + radiusHalf >= yQuad + quadHeight)
		return false;

	return true;
}

bool game::CollisionSystem::IntersectsOther(const int32_t a, const int32_t b,
	const Collider& aCollider, const Transform& aWorld,
	const Collider& bCollider, const Transform& bWorld,
	HitInfo& aInfo, HitInfo& bInfo)
{
	// Circle collision.
	const float threshold = aCollider.radius / 2 + bCollider.radius / 2;
	const auto intersection = aWorld.position - bWorld.position;
	const float intMagnitude = intersection.Magnitude();

	// Check if in range.
	if (intMagnitude > threshold)
		return false;

	// Set up hit info.
	aInfo.point = intersection;
	bInfo.point = intersection * -1;

	auto& aInstance = aInfo.instance;
	auto& bInstance = bInfo.instance;

	aInstance.index = a;
	bInstance.index = b;

	aInstance.collider = &aCollider;
	bInstance.collider = &bCollider;

	aInstance.world = &aWorld;
	bInstance.world = &bWorld;

	// Copy instance info to other hit info.
	aInfo.other = bInstance;
	bInfo.other = aInstance;

	return true;
}
