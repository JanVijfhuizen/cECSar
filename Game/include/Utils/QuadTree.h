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
		explicit constexpr QuadTree(const Quad& quad, int32_t depth = 8);

		template <typename Lambda>
		constexpr bool TryPush(T& instance, const Lambda&& method);
		template <typename Lambda>
		constexpr void Navigate(T& instance, const Lambda&& method, std::vector<T*>& out);
		constexpr void Clear();

	private:
		class Node final
		{
		public:
			int32_t depth = 0;
			Quad quad{};
			std::vector<T*> instances{};

			template <typename Lambda>
			constexpr Node* TryNavigate(
				T& instance, const Lambda&& method, std::vector<T*>* out);
			template <typename Lambda>
			constexpr void TrySplit(Pool<Node>& pool, const Lambda&& method);

			template <typename Lambda>
			constexpr void Push(T& instance, Pool<Node>& pool, const Lambda&& method);

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
	template <typename Lambda>
	constexpr bool QuadTree<T, C>::TryPush(T& instance, const Lambda&& method)
	{
		Node* node = _root.TryNavigate(instance, method, nullptr);
		if (node)
			node->Push(instance, _pool, method);
		return node;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Navigate(
		T& instance, const Lambda&& method, std::vector<T*>& out)
	{
		_root.TryNavigate(instance, method, &out);
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Clear()
	{
		_root.Clear(_pool);
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr typename QuadTree<T, C>::Node* QuadTree<T, C>::Node::TryNavigate(
		T& instance, const Lambda&& method, std::vector<T*>* out)
	{
		// Try to pass it to it's nested leaves/branches.
		if (!_isLeaf)
			for (auto i = 0; i < C; ++i)
			{
				Node* nested = _nested[i]->TryNavigate(instance, method, out);
				if (nested) 
				{
					// If out is not requested.
					if (!out)
						return nested;

					for (auto& inst : instances)
						out->push_back(inst);
					return nested;
				}
			}

		// Fitness check.
		const bool fit = method(instance, quad);
		if (!fit)
			return nullptr;

		// If out is not requested.
		if (!out)
			return this;

		for (auto& inst : instances)
			out->push_back(inst);
		return this;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::TrySplit(
		Pool<Node>& pool, const Lambda&& method)
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

				if (!nested->TryNavigate(instance, method, nullptr))
					continue;

				nested->Push(instance, pool, method);
				instances.erase(instances.begin() + j);
			}
		}
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::Push(
		T& instance, Pool<Node>& pool, const Lambda&& method)
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
