#pragma once
#include <typeindex>
#include <SparseSet.h>
#include <unordered_map>

#include "IEntityFactory.h"
#include <IComponentSystem.h>
#include <IModule.h>
#include <memory>

namespace cecsar
{
	struct CecsarInfo final
	{
		int32_t setCapacity = 256;
	};

	class Cecsar final
	{
	public:
		const CecsarInfo info;

		explicit inline Cecsar(CecsarInfo info = CecsarInfo());
		~Cecsar();

#pragma region System Management
		template <typename System>
		void Update();

		template <typename System>
		System& GetSystem();
#pragma endregion 

#pragma region Entity Management
		template <typename Factory = IEntityFactory>
		std::shared_ptr<int32_t[]> AddEntity(int32_t count = 1);

		void RemoveEntity(int32_t index);
#pragma endregion 

#pragma region Component Management
		template <typename Component>
		Component& AddComponent(int32_t index);

		template <typename Component>
		void RemoveComponent(int32_t index);
#pragma endregion 

#pragma region Other Getters
		template <typename Component>
		constexpr utils::SparseSet<Component>& GetSet();

		template <typename Module>
		constexpr Module& GetModule();

		template <typename Factory>
		IEntityFactory& GetFactory();
#pragma endregion

	private:
#pragma region Structs
		template <typename I>
		struct IMap
		{
			~IMap();

			template <typename T>
			T& Get(Cecsar& cecsar);

		private:
			std::unordered_map<std::type_index, I*> _map;
		};

		struct ISetContainer
		{
			virtual ~ISetContainer();
			virtual void RemoveAt(int32_t index) = 0;
		};

		template <typename Block>
		struct SetContainer : ISetContainer
		{
			utils::SparseSet<Block> set;

			SetContainer(int32_t capacity);
			void RemoveAt(int32_t index) override;
		};
#pragma endregion 

		utils::SparseSet<void*> _entities;
		std::unordered_map<std::type_index, ISetContainer*> _sets;

		IMap<IComponentSystem> _systems;
		IMap<IModule> _modules;
		IMap<IEntityFactory> _factories;
	};

	template <typename Component>
	constexpr utils::SparseSet<Component>& Cecsar::GetSet()
	{
		if (_sets.count(typeid(Component)) == 0)
		{
			auto set = new SetContainer<Component>(info.setCapacity);
			_sets[typeid(Component)] = set;
		}

		auto set = _sets[typeid(Component)];
		return static_cast<SetContainer<Component>*>(set)->set;
	}

	template <typename Module>
	constexpr Module& Cecsar::GetModule()
	{
		return _modules.Get<Module>(*this);
	}

	inline Cecsar::Cecsar(const CecsarInfo info) : 
		info(info), _entities(info.setCapacity)
	{
		
	}

	inline Cecsar::~Cecsar()
	{
		for (auto set : _sets)
			delete set.second;
	}

	template <typename System>
	void Cecsar::Update()
	{
		auto& sys = GetSystem<System>();
		static_cast<IComponentSystem*>(&sys)->Update(*this);
	}

	template <typename System>
	System& Cecsar::GetSystem()
	{
		return _systems.Get<System>(*this);
	}

	template <typename Factory>
	std::shared_ptr<int32_t[]> Cecsar::AddEntity(const int32_t count)
	{
		std::shared_ptr<int32_t[]> ptr(new int32_t[count]);

		for (int32_t i = 0; i < count; ++i)
		{
			const int32_t index = _entities.Add();
			ptr[i] = index;
		}

		if (typeid(Factory) != typeid(IEntityFactory)) 
		{
			auto& factory = GetFactory<Factory>();
			for (int32_t i = 0; i < count; ++i)
				factory.Construct(*this, ptr[i]);
		}

		return ptr;
	}

	inline void Cecsar::RemoveEntity(const int32_t index)
	{
		for (auto set : _sets)
			set.second->RemoveAt(index);
		_entities.RemoveAt(index);
	}

	inline Cecsar::ISetContainer::~ISetContainer() = default;

	template <typename Factory>
	IEntityFactory& Cecsar::GetFactory()
	{
		return _factories.Get<Factory>(*this);
	}

	template <typename Component>
	Component& Cecsar::AddComponent(const int32_t index)
	{
		return GetSet<Component>().Insert(index);
	}

	template <typename Component>
	void Cecsar::RemoveComponent(const int32_t index)
	{
		GetSet<Component>().RemoveAt(index);
	}

	template <typename I>
	Cecsar::IMap<I>::~IMap()
	{
		for (auto set : _map)
			delete set.second;
	}

	template <typename I>
	template <typename T>
	T& Cecsar::IMap<I>::Get(Cecsar& cecsar)
	{
		if (_map.count(typeid(T)) == 0)
		{
			auto t = new T();
			static_cast<I*>(t)->Initialize(cecsar);
			_map[typeid(T)] = t;
		}

		const auto t = _map[typeid(T)];
		return *static_cast<T*>(t);
	}

	template <typename Component>
	Cecsar::SetContainer<Component>::SetContainer(const int32_t capacity): set(capacity)
	{
	}

	template <typename Block>
	void Cecsar::SetContainer<Block>::RemoveAt(const int32_t index)
	{
		set.RemoveAt(index);
	}
}
