#pragma once
#include "Pool.h"

namespace utils
{
	template <typename T, size_t C = 4>
	class QuadTree final
	{
	public:
		constexpr void Push(T& instance);
		constexpr std::vector<T*>& Navigate(T& instance);
		constexpr void Clear();

	private:
		class Node final
		{
		public:
			std::vector<T*> instances{};

			constexpr Node* TryNavigate(T& instance, Pool<Node>& pool);
			constexpr void Push(T& instance);
			constexpr void Clear(Pool<Node>& pool);

		private:
			Node* _nested[C]{};
			bool _isLeaf = true;
		};

		Pool<Node> _pool{};
		Node _root{};
	};

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Push(T& instance)
	{
		Node* node = _root.TryNavigate(instance, _pool);
		node->Push(instance);
	}

	template <typename T, size_t C>
	constexpr std::vector<T*>& QuadTree<T, C>::Navigate(T& instance)
	{
		Node* node = _root.TryNavigate(instance, _pool);
		return node->instances;
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Clear()
	{
		_root.Clear(_pool);
	}

	template <typename T, size_t C>
	constexpr typename QuadTree<T, C>::Node* QuadTree<T, C>::Node::TryNavigate(
		T& instance, Pool<Node>& pool)
	{
		// Check if the current leaf is out of capacity.
		if (instances.size() >= 4)
		{
			// Change into a branch.
			if (_isLeaf)
			{
				_isLeaf = false;
				for (auto i = 0; i < C; ++i)
					_nested[i] = &pool.Get();
			}
		}

		// Try to pass it to it's nested leaves/branches.
		if (!_isLeaf)
			for (auto i = 0; i < C; ++i)
			{
				Node* nested = _nested[i]->TryNavigate(instance, pool);
				if(nested)
					return nested;
			}

		// Fitness check.

		return this;
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Node::Push(T& instance)
	{
		instances.push_back(&instance);
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
}
