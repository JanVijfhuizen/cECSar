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

		template <typename System>
		void Update();

		template <typename System>
		System& GetSystem();

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

#pragma region Getters
		template <typename Component>
		constexpr utils::SparseSet<Component>& GetSet();

		template <typename Module>
		constexpr Module& GetModule();
#pragma endregion

	private:
#pragma region Structs
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

#define INDEX_MAP(type) std::unordered_map<std::type_index, type*>

		INDEX_MAP(ISetContainer) _sets;
		INDEX_MAP(IComponentSystem) _systems;
		INDEX_MAP(IModule) _modules;
		INDEX_MAP(IEntityFactory) _factories;

#pragma region Getters
		template <typename Factory>
		IEntityFactory& GetFactory();
#pragma endregion 
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
		if (_modules.count(typeid(Module)) == 0)
		{
			auto module = new Module();
			static_cast<IModule*>(module)->Initialize(*this);
			_modules[typeid(Module)] = module;
		}

		const auto set = _modules[typeid(Module)];
		return *static_cast<Module*>(set);
	}

	inline Cecsar::Cecsar(const CecsarInfo info) : 
		info(info), _entities(info.setCapacity)
	{
		
	}

	inline Cecsar::~Cecsar()
	{
		for (auto set : _sets)
			delete set.second;
		for (auto system : _systems)
			delete system.second;
		for (auto module : _modules)
			delete module.second;
		for (auto factory : _factories)
			delete factory.second;
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
		if (_systems.count(typeid(System)) == 0)
		{
			auto sys = new System();
			static_cast<IComponentSystem*>(sys)->Initialize(*this);
			_systems[typeid(System)] = sys;
		}

		const auto sys = _systems[typeid(System)];
		return *static_cast<System*>(sys);
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
		if (_factories.count(typeid(Factory)) == 0)
		{
			auto factory = new Factory();
			static_cast<IEntityFactory*>(factory)->Initialize(*this);
			_factories[typeid(Factory)] = factory;
		}

		const auto factory = _factories[typeid(Factory)];
		return *factory;
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
