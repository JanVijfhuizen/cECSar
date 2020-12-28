#pragma once
#include <typeindex>
#include <SparseSet.h>
#include <unordered_map>

#include "IEntityFactory.h"
#include <IComponentSystem.h>
#include <IModule.h>
#include <memory>
#include <EntityInfo.h>

namespace cecsar
{
	/*
	Settings for the ECS framework.
	Cecsar uses the default values if no settings are overloaded during construction.
	*/
	struct CecsarSettings final
	{
		/*
		The capacity of each sparse set.
		Setting this high doesn't directly affect performance, but it drastically affects
		startup times.

		Take note that this system uses lazy initialization and that not everything might be
		set up in the first frame.

		To force initialization, you can just use GetSet on every set you want initialized.
		*/
		int32_t setCapacity = 256;
	};

	/*
	ECS framework.
	Manages everything that is related to ECS.
	Uses lazy initialization for everything.
	*/
	class Cecsar final
	{
	public:
		const CecsarSettings info;

		explicit inline Cecsar(CecsarSettings info = CecsarSettings());
		~Cecsar();

#pragma region System Management
		/*
		Updates target system.
		Systems manage one or more components.
		A TransformSystem would, for instance, modify the Transform components in some way.

		Inherit your custom system from ComponentSystem to make use of this feature.
		*/
		template <typename System>
		void Update();

		/*
		Gets target system.
		Read the update comment for more info on systems.
		Sometimes systems have specific functionality (ie TransformSystem: SetParent),
		so just having an update function is not enough.
		*/
		template <typename System>
		System& GetSystem();
#pragma endregion 

#pragma region Entity Management
		/*
		Adds N entities and returns its indexes as a shared pointer.
		The index can be used as an overload to add/remove components.
		A factory type can be given, but is not required.

		Read the GetFactory description for more info.
		*/
		template <typename Factory = IEntityFactory>
		std::shared_ptr<EntityInfo[]> AddEntity(int32_t count = 1);

		/*
		Get target entity's global id.
		*/
		inline int32_t GetEntity(int32_t index);

		inline bool IsEntityValid(const EntityInfo& entity);

		/*
		Removes an entity from the game.
		This also removes all attached components.
		*/
		void RemoveEntity(int32_t index);
#pragma endregion 

#pragma region Component Management
		/*
		Add a component to a target entity.
		*/
		template <typename Component>
		Component& AddComponent(int32_t index);

		/*
		Removes a component from a target entity.
		*/
		template <typename Component>
		void RemoveComponent(int32_t index);
#pragma endregion 

#pragma region Other Getters
		/*
		Get the sparse set of components of type "Component".
		A sparse set is not the most common type of data structure,
		so you might want to read up on it a bit.

		Essentially you can iterate over it, but you can also it as a hashset/map.
		Example:

		for(auto& component : components)
			component.x++;

		OR

		const int32_t count = components.GetCount();
		for(int32_t i = 0; i < count; ++i)
			components[i].x++;

		OR

		const int32_t targetEntity = 25;
		components.Get(targetEntity).x++;

		*/
		template <typename Component>
		constexpr utils::SparseSet<Component>& GetSet();

		/*
		Gets target module.
		A module can be anything at all, as long as it inherits from the IModule interface.

		This is useful for when you want to have a class that manages something that doesn't
		immediately relate to ECS, but you still want to have access to it.

		Example:
		JobSystem, TimeModule, RenderModule, TextureLib etc.
		*/
		template <typename Module>
		constexpr Module& GetModule();

		/*
		Gets target factory.

		You can use a factory to streamline object creation,
		by predefining what components need to be created with what values.
		*/
		template <typename Factory>
		IEntityFactory& GetFactory();

		/*
		Gets uniquely defined ID for entity with target index.
		Since entities can be created/destroyed, using an index to check if something
		is still there isn't reccomended.
		*/
		constexpr int32_t GetEntityId(int32_t index);
#pragma endregion

	private:
#pragma region Structs
		template <typename I>
		struct IMap final
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
		struct SetContainer final : ISetContainer
		{
			utils::SparseSet<Block> set;

			SetContainer(int32_t capacity);
			void RemoveAt(int32_t index) override;
		};
#pragma endregion 

		utils::SparseSet<int32_t> _entities;
		int32_t _globalEntityCount = 0;

		std::unordered_map<std::type_index, ISetContainer*> _sets;

		IMap<IComponentSystem> _systems;
		IMap<IModule> _modules;
		IMap<IEntityFactory> _factories;
	};

	constexpr int32_t Cecsar::GetEntityId(const int32_t index)
	{
		return _entities.Get(index);
	}

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

	inline Cecsar::Cecsar(const CecsarSettings info) : 
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
	std::shared_ptr<EntityInfo[]> Cecsar::AddEntity(const int32_t count)
	{
		std::shared_ptr<EntityInfo[]> ptr(new EntityInfo[count]);

		for (int32_t i = 0; i < count; ++i)
		{
			const int32_t index = _entities.Add(_globalEntityCount);
			ptr[i] = { index, _globalEntityCount++ };
		}

		if (typeid(Factory) != typeid(IEntityFactory)) 
		{
			auto& factory = GetFactory<Factory>();
			for (int32_t i = 0; i < count; ++i)
				factory.Construct(*this, ptr[i]);
		}

		return ptr;
	}

	inline int32_t Cecsar::GetEntity(const int32_t index)
	{
		return _entities.Get(index);
	}

	inline bool Cecsar::IsEntityValid(const EntityInfo& entity)
	{
		if (entity.index < 0 || entity.index >= info.setCapacity)
			return false;
		return _entities.Get(entity.index) == entity.globalId;
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
