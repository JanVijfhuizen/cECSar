#pragma once
#include "Pool.h"

namespace utils
{
	struct Quad final
	{
		Vector3 pos{};
		float width = 0, height = 0;

		constexpr Quad();
		inline Quad(const Vector3& pos, float width, float height);
	};

	template <typename T, size_t C = 4>
	class QuadTree final
	{
	public:
		using NavMethod = std::function<bool(const T&, const Quad&)>;

		explicit constexpr QuadTree(const Quad& quad, int32_t depth = 8);

		constexpr bool TryPush(T& instance, const NavMethod& method);
		constexpr std::vector<T*>* TryNavigate(T& instance, const NavMethod& method);
		constexpr void Clear();

	private:
		class Node final
		{
		public:
			int32_t depth = 0;
			Quad quad{};
			std::vector<T*> instances{};

			constexpr Node* TryNavigate(T& instance, const NavMethod& method);
			constexpr void TrySplit(Pool<Node>& pool, const NavMethod& method);

			constexpr void Push(T& instance, 
				Pool<Node>& pool, const NavMethod& method);
			constexpr void Clear(Pool<Node>& pool);

		private:
			Node* _nested[C]{};
			bool _isLeaf = true;
		};

		Pool<Node> _pool{};
		Node _root{};
	};

	template <typename T, size_t C>
	constexpr QuadTree<T, C>::QuadTree(const Quad& quad, const int32_t depth)
	{
		_root.quad = quad;
		_root.depth = depth;
	}

	template <typename T, size_t C>
	constexpr bool QuadTree<T, C>::TryPush(
		T& instance, const NavMethod& method)
	{
		Node* node = _root.TryNavigate(instance, method);
		if(node)
			node->Push(instance, _pool, method);
		return node;
	}

	template <typename T, size_t C>
	constexpr std::vector<T*>* QuadTree<T, C>::TryNavigate(
		T& instance, const NavMethod& method)
	{
		Node* node = _root.TryNavigate(instance, method);
		return node ? &node->instances : nullptr;
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Clear()
	{
		_root.Clear(_pool);
	}

	template <typename T, size_t C>
	constexpr typename QuadTree<T, C>::Node* QuadTree<T, C>::Node::TryNavigate(
		T& instance, const NavMethod& method)
	{
		// Try to pass it to it's nested leaves/branches.
		if (!_isLeaf)
			for (auto i = 0; i < C; ++i)
			{
				Node* nested = _nested[i]->TryNavigate(instance, method);
				if(nested)
					return nested;
			}

		// Fitness check.
		return method(instance, quad) ? this : nullptr;
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Node::TrySplit(
		Pool<Node>& pool, const NavMethod& method)
	{
		if (depth == 0)
			return;
		if (!_isLeaf)
			return;
		if (instances.size() < C)
			return;

		// Check if the current leaf is out of capacity.
		_isLeaf = false;
		for (auto i = 0; i < C; ++i)
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

			// Emplace all instances in nested nodes.
			for (int32_t j = instances.size() - 1; j >= 0; --j)
			{
				auto& instance = *instances[j];

				if (!nested->TryNavigate(instance, method))
					continue;

				nested->Push(instance, pool, method);
				instances.erase(instances.begin() + j);
			}
		}
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Node::Push(
		T& instance, Pool<Node>& pool, const NavMethod& method)
	{
		instances.push_back(&instance);
		TrySplit(pool, method);
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Node::Clear(Pool<Node>& pool)
	{
		// Clear self.
		if (_isLeaf)
		{
			instances.clear();
			return;
		}

		// Clear nested branches.
		_isLeaf = true;
		for (auto i = 0; i < C; ++i)
		{
			_nested[i]->Clear(pool);
			pool.Push(*_nested[i]);
		}
	}

	constexpr Quad::Quad() = default;

	inline Quad::Quad(const Vector3& pos, const float width, const float height) :
		pos(pos), width(width), height(height)
	{
	}
}
