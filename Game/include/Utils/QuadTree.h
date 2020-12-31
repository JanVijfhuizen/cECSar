#pragma once
#include <Utils/Pool.h>

namespace utils
{
	struct Quad final
	{
		Vector3 pos{};
		float width = 0, height = 0;

		constexpr Quad();
		inline Quad(const Vector3& pos, float width, float height);
	};

	template <typename T, size_t C = 16>
	class QuadTree final
	{
	public:
		using Instances = std::vector<std::vector<T*>*>;

		explicit constexpr QuadTree(float width, float height, int32_t depth = 8);

		template <typename Lambda>
		constexpr bool TryPush(T& instance, const Lambda& method);

		template <typename Lambda>
		constexpr void Iterate(const Lambda&& method);

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
				T& instance, const Lambda& method);
			template <typename Lambda>
			constexpr void TrySplit(Pool<Node>& pool, const Lambda& method);

			template <typename Lambda>
			constexpr void Push(T& instance, Pool<Node>& pool, const Lambda& method);

			constexpr bool Clear(Pool<Node>& pool);

			template <typename Lambda>
			constexpr void Iterate(Instances& vector, Lambda&& method);

		private:
			Node* _nested[4]{};
			bool _isLeaf = true;
		};

		Pool<Node> _pool{};
		Node _root{};

		template <typename Lambda>
		constexpr void Navigate(T& instance, const Lambda&& method);
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
	constexpr bool QuadTree<T, C>::TryPush(T& instance, const Lambda& method)
	{
		Node* node = _root.TryNavigate(instance, method);
		if (node)
			node->Push(instance, _pool, method);
		return node;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Navigate(
		T& instance, const Lambda&& method)
	{
		_root.TryNavigate(instance, method);
	}

	template <typename T, size_t C>
	constexpr void QuadTree<T, C>::Clear()
	{
		_root.Clear(_pool);
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Iterate(const Lambda&& method)
	{
		Instances instances{};
		_root.Iterate(instances, method);
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr typename QuadTree<T, C>::Node* QuadTree<T, C>::Node::TryNavigate(
		T& instance, const Lambda& method)
	{
		// Try to pass it to it's nested leaves/branches.
		if(!_isLeaf)
			for (auto& i : _nested)
			{
				Node* nested = i->TryNavigate(instance, method);
				if (nested)
					return nested;
			}

		// Fitness check.
		const bool fit = method(instance, quad);
		if (!fit)
			return nullptr;
		return this;
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::TrySplit(
		Pool<Node>& pool, const Lambda& method)
	{
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
				if (!nested->TryNavigate(instance, method))
					continue;

				nested->Push(instance, pool, method);
				instances.erase(instances.begin() + i);
				break;
			}
		}
	}

	template <typename T, size_t C>
	template <typename Lambda>
	constexpr void QuadTree<T, C>::Node::Push(
		T& instance, Pool<Node>& pool, const Lambda& method)
	{
		instances.push_back(&instance);
		TrySplit(pool, method);
	}

	template <typename T, size_t C>
	constexpr bool QuadTree<T, C>::Node::Clear(Pool<Node>& pool)
	{
		bool empty = instances.empty();
		instances.clear();

		// Clear nested branches.
		if (!_isLeaf)
		{
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
		Instances& vector, Lambda&& method)
	{
		vector.push_back(&instances);

		// Recursively go though branches.
		// Only call the method at the leafs.
		if (_isLeaf)
			method(vector);
		else
			for (auto& nested : _nested)
				nested->Iterate(vector, method);

		vector.pop_back();
	}

	constexpr Quad::Quad() = default;

	inline Quad::Quad(const Vector3& pos, const float width, const float height) :
		pos(pos), width(width), height(height)
	{
	}
}
