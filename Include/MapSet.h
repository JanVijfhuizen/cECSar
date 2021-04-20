#pragma once
#include "Set.h"
#include <unordered_map>
#include <fstream>

namespace jecs
{
	// A specialized set that functions like a map.
	template <typename T>
	class MapSet final : public Set<MapSet<T>>
	{
	public:
		struct Value final
		{
			T& value;
			const int32_t index;
		};

		class Iterator final
		{
		public:
			using It = typename std::unordered_map<int, T>::iterator;

			Iterator(MapSet<T>& set, It it);

			Value operator*() const;
			Value operator->() const;

			const Iterator& operator++();
			Iterator operator++(int);

			friend auto operator==(const Iterator& a, const Iterator& b) -> bool
			{
				return a._it == b._it;
			};

			friend bool operator!= (const Iterator& a, const Iterator& b)
			{
				return !(a == b);
			};

		private:
			typename std::unordered_map<int, T>::iterator _it{};
			MapSet<T>& _set;
		};

		explicit MapSet();

		[[nodiscard]] constexpr T& operator[](int32_t index);

		constexpr T& Insert(int32_t index, const T& val);
		constexpr T& Insert(int32_t index, T&& val = T());

		void EraseAt(int32_t index) override;
		void Clear();

		[[nodiscard]] std::unordered_map<int32_t, T>& GetMapRaw();

		[[nodiscard]] constexpr Iterator begin();
		[[nodiscard]] constexpr Iterator end();

		// Load set to disk.
		void Load();
		// Save set to disk.
		void Save();

	private:
		std::unordered_map<int32_t, T> _map{};
	};

	template <typename T>
	constexpr T& MapSet<T>::operator[](const int32_t index)
	{
		return _map[index];
	}

	template <typename T>
	constexpr T& MapSet<T>::Insert(const int32_t index, const T& val)
	{
		return _map[index] = val;
	}

	template <typename T>
	constexpr T& MapSet<T>::Insert(const int32_t index, T&& val)
	{
		return _map[index] = std::move(val);
	}

	template <typename T>
	constexpr typename MapSet<T>::Iterator MapSet<T>::begin()
	{
		return Iterator{ *this, _map.begin() };
	}

	template <typename T>
	constexpr typename MapSet<T>::Iterator MapSet<T>::end()
	{
		return Iterator{ *this, _map.end() };
	}

	template <typename T>
	MapSet<T>::Iterator::Iterator(MapSet<T>& set, It it) :
		_it(it), _set(set)
	{

	}

	template <typename T>
	typename MapSet<T>::Value MapSet<T>::Iterator::operator*() const
	{
		auto& pair = *_it;
		return {pair.second, pair.first };
	}

	template <typename T>
	typename MapSet<T>::Value MapSet<T>::Iterator::operator->() const
	{
		auto& pair = *_it;
		return { pair.second, pair.first };
	}

	template <typename T>
	const typename MapSet<T>::Iterator& MapSet<T>::Iterator::operator++()
	{
		++_it;
		return *this;
	}

	template <typename T>
	typename MapSet<T>::Iterator MapSet<T>::Iterator::operator++(int)
	{
		Iterator temp{ *this };
		++_it;
		return temp;
	}

	template <typename T>
	MapSet<T>::MapSet()
	{
		if (Cecsar::Get().loadFromFile)
			Load();
	}

	template <typename T>
	void MapSet<T>::EraseAt(const int32_t index)
	{
		_map.erase(index);
	}

	template <typename T>
	void MapSet<T>::Clear()
	{
		_map.clear();
	}

	template <typename T>
	std::unordered_map<int32_t, T>& MapSet<T>::GetMapRaw()
	{
		return _map;
	}

	template <typename T>
	void MapSet<T>::Load()
	{
		Clear();

		std::ifstream file;
		file.open(Set<T>::template GetFilePath<T>(), std::ios::in);
		if (!file.good())
			return;

		while (!file.eof())
		{
			int32_t key;
			T value;

			file.read(reinterpret_cast<char*>(&key), sizeof(int32_t));
			file.read(reinterpret_cast<char*>(&value), sizeof(T));
			_map[key] = std::move(value);
		}
	}

	template <typename T>
	void MapSet<T>::Save()
	{
		std::ofstream file;
		file.open(Set<T>::template GetFilePath<T>(), std::ios::out);

		for (auto& keyPair : _map)
		{
			int32_t first = keyPair.first;
			T second = keyPair.second;

			file.write(reinterpret_cast<char*>(&first), sizeof(int32_t));
			file.write(reinterpret_cast<char*>(&second), sizeof(T));
		}
	}
}
