#pragma once
#include <Utils/Pool.h>

namespace utils
{
constexpr auto QUAD_BLOCK_SIZE = 4;

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
	class QuadTree final
	{
	public:
		/*
		Class that recursively nests to create the tree.
		Holds instances.
		*/
		class Node final
		{
			friend QuadTree;

		public:
			Quad quad{};
			std::vector<int32_t> instances{};

		private:
			Node* _nested[4]{};
			int32_t _depth = 0;
			bool _isLeaf = true;

			/*
			Recursively iterate through the nodes.
			If the lambda returns true, the instance fits within this node.
			*/
			template <typename Lambda>
			constexpr Node* TryNavigate(int32_t instance, const Lambda& lambda);
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
			constexpr void Push(int32_t instance, Pool<Node>& pool, const Lambda& lambda);
			/*
			Recursively iterate through all the groups and use them as a parameter
			in the overloaded lambda.

			Rather than constantly filling a single vector every single frame,
			I decided to just return a vector of instances.

			This way I only have to refill a single vector instead of copying all those
			entity references, which is costly to say the least.

			It makes using this algorithms more convoluted, but in the end, this algorithm is
			ment to increase performance, so that's what I'll do.
			*/
			template <typename Lambda>
			constexpr void Iterate(std::vector<Node*>& vector, 
				int32_t& anchor, int32_t maxDepth, Lambda&& lambda);

			/*
			Clear the node. Doest pool the node if it's not empty.
			*/
			inline bool Clear(Pool<Node>& pool, bool clearInstances);
		};

		/*
		Overload the width and height of the initial quad.
		The depths corresponds to the maximum depth for nested objects.
		*/
		inline QuadTree(const Vector3& origin, float width, float height, int32_t depth = 8);

		/*
		Push an object into the quadtree, while using a lambda to sort it correctly.
		*/
		template <typename Lambda>
		constexpr bool TryPush(int32_t instance, const Lambda& lambda);
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
		inline void Clear(bool clearInstances);

	private:
		Pool<Node> _pool{};
		Node _root{};
	};

	inline QuadTree::QuadTree(const Vector3& origin, 
		const float width, const float height, const int32_t depth)
	{
		Quad&& quad{ origin, width, height };

		_root.quad = quad;
		_root._depth = depth;
	}

	template <typename Lambda>
	constexpr bool QuadTree::TryPush(int32_t instance, const Lambda& lambda)
	{
		Node* node = _root.TryNavigate(instance, lambda);
		if (node)
			node->Push(instance, _pool, lambda);
		return node;
	}

	inline void QuadTree::Clear(const bool clearInstances)
	{
		_root.Clear(_pool, clearInstances);
	}

	template <typename Lambda>
	constexpr void QuadTree::Iterate(const Lambda&& lambda)
	{
		std::vector<Node*> instances{};
		int32_t anchor = 0;
		_root.Iterate(instances, anchor, _root._depth, lambda);
	}

	template <typename Lambda>
	constexpr QuadTree::Node* QuadTree::Node::TryNavigate(
		int32_t instance, const Lambda& lambda)
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

	template <typename Lambda>
	constexpr void QuadTree::Node::TrySplit(
		Pool<Node>& pool, const Lambda& lambda)
	{
		// Check if it's splittable.
		if (!_isLeaf)
			return;
		if (_depth == 0)
			return;
		if (instances.size() <= QUAD_BLOCK_SIZE)
			return;

		_isLeaf = false;

		// Create nested nodes.
		for (auto i = 0; i < 4; ++i)
		{
			auto& nested = _nested[i] = &pool.Get();
			nested->_depth = _depth - 1;

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
			auto& instance = instances[i];

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

	template <typename Lambda>
	constexpr void QuadTree::Node::Push(
		int32_t instance, Pool<Node>& pool, const Lambda& lambda)
	{
		instances.push_back(instance);
		TrySplit(pool, lambda);
	}

	inline bool QuadTree::Node::Clear(Pool<Node>& pool, const bool clearInstances)
	{
		bool empty = instances.empty();
		if(clearInstances)
			instances.clear();

		// If this doesn't have nested nodes.
		if (_isLeaf)
			return empty;

		// Clear nested branches.
		// Check if every child node is empty.
		for (auto& i : _nested)
		{
			auto nested = i;
			const bool nestedEmpty = nested->Clear(pool, clearInstances);
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

		return empty;
	}

	template <typename Lambda>
	constexpr void QuadTree::Node::Iterate(std::vector<Node*>& vector, 
		int32_t& anchor, const int32_t maxDepth, Lambda&& lambda)
	{
		vector.push_back(this);

		// Recursively go though branches.
		// Only call the lambda at the leafs.
		if (_isLeaf)
			lambda(vector, anchor);
		else
			for (auto& nested : _nested)
				nested->Iterate(vector, anchor, maxDepth, lambda);

		vector.pop_back();
		anchor = maxDepth - _depth;
	}

	constexpr Quad::Quad() = default;


inline Quad::Quad(const Vector3& pos, const float width, const float height) :
		pos(pos), width(width), height(height)
	{

	}
}
