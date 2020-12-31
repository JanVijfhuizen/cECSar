#pragma once
#include <Utils/Pool.h>

namespace utils
{
	/*
	Used for intersection checks in the QuadTree.
	*/
	struct Quad final
	{
		Vector3 pos{};
		float width = 0, height = 0;

		constexpr Quad();
		inline Quad(const Vector3& pos, float width, float height);
	};

	/*
	Used to decrease the amount of intersection checks needed for large groups.

	It accomplishes this by dividing the surface into multiple smaller quads,
	and assigning the intersectable objects to their corresponding quad, where they only
	need to check collisions with the objects in the same quad.
	*/
	template <typename T, size_t C = 16>
	class QuadTree final
	{
	public:
		/*
		Rather than constantly filling a single vector every single frame,
		I decided to just return a vector of instances.

		This way I only have to refill a single vector instead of copying all those
		entity references, which is costly to say the least.

		It makes using this algorithms more convoluted, but in the end, this algorithm is
		ment to increase performance, so that's what I'll do.
		*/
		using Instances = std::vector<std::vector<T*>*>;

		/*
		Overload the width and height of the initial quad.
		The depths corresponds to the maximum depth for nested objects.
		*/
		explicit constexpr QuadTree(float width, float height, int32_t depth = 8);

		/*
		Push an object into the quadtree, while using a lambda to sort it correctly.
		*/
		template <typename Lambda>
		constexpr bool TryPush(T& instance, const Lambda& lambda);
		/*
		Iterate over all the different groups in the tree.
		Do please look at the way Instances is constructed, since it's done in a bit
		of a weird way to save performance.
		*/
		template <typename Lambda>
		constexpr void Iterate(const Lambda&& lambda);

		/*
		Clear the tree.
		It only pools nodes that are empty, to save performance when filling/clearing frequently.
		To save ALL the nodes removed regardless, simply clear twice.
		*/
		constexpr void Clear();

	private:
		/*
		Internal class that recursively nests to create the tree.
		Holds instances.
		*/
		class Node final
		{
		public:
			int32_t depth = 0;
			Quad quad{};
			std::vector<T*> instances{};

			/*
			Recursively iterate through the nodes.
			If the lambda returns true, the instance fits within this node.
			*/
			template <typename Lambda>
			constexpr Node* TryNavigate(T& instance, const Lambda& lambda);
			/*
			Try splitting the node into multiple sub nodes.
			Done when there are too many instances in the node.
			*/
			template <typename Lambda>
			constexpr void TrySplit(Pool<Node>& pool, const Lambda& lambda);

			/*
			Push an instance to this node.
			Also check if the node needs to split.
			*/
			template <typename Lambda>
			constexpr void Push(T& instance, Pool<Node>& pool, const Lambda& lambda);
			/*
			Recursively iterate through all the groups and use them as a parameter
			in the overloaded lambda.
			*/
			template <typename Lambda>
			constexpr void Iterate(Instances& vector, Lambda&& lambda);

			/*
			Clear the node. Doest pool the node if it's not empty.
			*/
			constexpr bool Clear(Pool<Node>& pool);

		private:
			Node* _nested[4]{};
			bool _isLeaf = true;
		};

		Pool<Node> _pool{};
		Node _root{};
	};

	template <typename T, size_t C>
	constexpr QuadTree<T, C>::QuadTree(const float width, const float height, const int32_t depth)
	{
		Quad&& quad{ {}, width, height };

		_root.quad = quad;
		_root.depth = depth;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr bool QuadTree<T, C>::TryPush(T& instance, const Lambda& lambda)
	{
		Node* node = _root.TryNavigate(instance, lambda);
		if (node)
			node->Push(instance, _pool, lambda);
		return node;
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Clear()
	{
		_root.Clear(_pool);
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Iterate(const Lambda&& lambda)
	{
		Instances instances{};
		_root.Iterate(instances, lambda);
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr typename QuadTree<T, C>::Node* QuadTree<T, C>::Node::TryNavigate(
		T& instance, const Lambda& lambda)
	{
		// Try to pass it to it's nested leaves/branches.
		if(!_isLeaf)
			for (auto& i : _nested)
			{
				Node* nested = i->TryNavigate(instance, lambda);
				if (nested)
					return nested;
			}

		// Fitness check.
		const bool fit = lambda(instance, quad);
		if (!fit)
			return nullptr;
		return this;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::TrySplit(
		Pool<Node>& pool, const Lambda& lambda)
	{
		// Check if it's splittable.
		if (!_isLeaf)
			return;
		if (depth == 0)
			return;
		if (instances.size() < C)
			return;

		_isLeaf = false;

		// Create nested nodes.
		for (auto i = 0; i < 4; ++i)
		{
			auto& nested = _nested[i] = &pool.Get();
			nested->depth = depth - 1;

			const bool isEven = i % 2 == 0;
			const float width = quad.width / 2;
			const float height = quad.height / 2;

			const auto offset = Vector3(width * isEven, height * (i > 1), 0);

			nested->quad =
			{
				quad.pos + offset,
				width,
				height
			};
		}

		// Emplace all instances in nested nodes.
		for (int32_t i = instances.size() - 1; i >= 0; --i)
		{
			auto& instance = *instances[i];

			for (auto& nested : _nested)
			{
				if (!nested->TryNavigate(instance, lambda))
					continue;

				nested->Push(instance, pool, lambda);
				instances.erase(instances.begin() + i);
				break;
			}
		}
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::Push(
		T& instance, Pool<Node>& pool, const Lambda& lambda)
	{
		instances.push_back(&instance);
		TrySplit(pool, lambda);
	}

	template <typename T, size_t C>
	constexpr bool QuadTree<T, C>::Node::Clear(Pool<Node>& pool)
	{
		bool empty = instances.empty();
		instances.clear();

		// Clear nested branches.
		if (!_isLeaf)
		{
			// Check if every child node is empty.
			for (auto& i : _nested)
			{
				auto nested = i;
				const bool nestedEmpty = nested->Clear(pool);
				if (!nestedEmpty)
					empty = false;
			}

			// Only pool the nodes if all child nodes are empty.
			if (empty)
			{
				_isLeaf = true;
				for (auto& i : _nested)
				{
					pool.Push(*i);
					i = nullptr;
				}
			}
		}

		return empty;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::Iterate(
		Instances& vector, Lambda&& lambda)
	{
		vector.push_back(&instances);

		// Recursively go though branches.
		// Only call the lambda at the leafs.
		if (_isLeaf)
			lambda(vector);
		else
			for (auto& nested : _nested)
				nested->Iterate(vector, lambda);

		vector.pop_back();
	}

	constexpr Quad::Quad() = default;

	inline Quad::Quad(const Vector3& pos, const float width, const float height) :
		pos(pos), width(width), height(height)
	{
	}
}
