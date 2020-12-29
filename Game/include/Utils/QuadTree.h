#pragma once
#include "Pool.h"

namespace utils
{
	template <typename T, size_t C = 4>
	class QuadTree final
	{
	public:
		constexpr void Push(T& instance);
		constexpr void Clear();

	private:
		class Node final
		{
		public:
			constexpr bool TryPush(T& instance, Pool<Node>& pool);
			constexpr void Clear(Pool<Node>& pool);

		private:
			std::vector<T*> _instances{};
			Node* _nested[C]{};
			bool _isLeaf = true;
		};

		Pool<Node> _pool;
		Node _root;
	};

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Push(T& instance)
	{
		_root.TryPush(instance, _pool);
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Clear()
	{
		_root.Clear(_pool);
	}

	template <typename T, size_t C>
	constexpr bool QuadTree<T, C>::Node::TryPush(
		T& instance, Pool<Node>& pool)
	{
		// Check if the current leaf is out of capacity.
		if(_instances.size() >= 4)
		{
			// Change into a branch.
			if(_isLeaf)
			{
				_isLeaf = false;
				for (auto i = 0; i < C; ++i)
					_nested[i] = &pool.Get();
			}
		}

		// Try to pass it to it's nested leaves/branches.
		if(!_isLeaf)
			for (auto i = 0; i < C; ++i)
				if (_nested[i]->TryPush(instance, pool))
					return true;

		// If it fits in none of the nested branches or the capacity hasn't been reached yet,
		// add it to this node.
		_instances.push_back(&instance);
		return true;
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Node::Clear(Pool<Node>& pool)
	{
		// Clear self.
		if (_isLeaf)
		{
			_instances.clear();
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
