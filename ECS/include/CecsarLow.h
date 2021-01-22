#pragma once
#include <cstdint>
#include <typeindex>
#include <unordered_map>

#include "SparseSet.h"

namespace revamped
{
	/*
	 This is a framework that streamlines the ECS workflow by simplifying it quite a bit.
	 The goal here is to make ECS as inviting as possible for people not yet used to
	 the workflow.
	 
	 This is also why there aren't a lot of features: I want simplicity.
	 You can always expand on it by writing a wrapper class.
	
	 For ease of use, this framework uses lazy initialization for everything.
	 So just create your System or Factory, inherit from the corresponding class and things will work out.

	BASICS:
	The framework works as follows:
	- You create the cecsar class, optionally overloading a Cecsar::Info struct with your preferences, like the entity capacity.
	- You're done.

	You can now use Spawn() to create a new entity, and Destroy() to...well destroy an entity.
	Spawn returns an entity struct, which holds two values: an index and a id.
	The index can be used to add/remove or access components.
	
	While one entity might have the same index as an entity that was destroyed earlier on, the ID is always unique.
	So always compare with the IDs, and not the indexes!
	
	You can grab the set you want, like set = GetSet<Renderer>().
	After that, you can use set.Insert(index), set.RemoveAt(index) or just set.Get(index) to adjust the component.

	INTERMEDIATE:
	While the basics are cool and all, the end user still has to write their own way of streamlining entity construction
	and component behaviour.
	Thia ia where both the System and the Factory base classes come into play.

	SYSTEMS are used to define behaviour for a set of components.
	A RenderSystem might manage the Renderer components for instance.
	You can create your custom System by inheriting like this: RenderSystem : public Cecsar::System<Renderer>.
	
	In the missing OnUpdate call, you can define your behaviour, which can be as simple as this:
	foreach(auto& renderer : instances) Render(renderer).
	After that, just call Get<RenderSystem>().Update() to use it.

	FACTORIES are used to streamline construction.
	While an entity is just a collection of components at runtime, you can define the way an entity can be created.
	You can, for instance, use a factory to predefine the creation of an Orc.
	To create a custom factory, use OrcFactory : public Cecsar::Factory.
	In the missing OnConstruction call, you can use the overloaded index to add components, and adjust them.
	
	It can be as simple as this:
	cecsar.GetSet<Collider>().Insert(index);
	auto& renderer = cecsar.GetSet<Renderer>().Insert(index).
	renderer.texture = orcTexture.
	Now you just need to call Get<OrcFactory>().Construct(index), and your orc construction process is streamlined!

	ADVANCED:
	ARGS in systems: Systems do usually manage only one component type, but often these components can be dependent on other components,
	like a Renderer is dependent on the Transform component.
	When defining your system, you can easily get access to these other sets by defining it like this:
	RenderSystem : public Cecsar::System<Renderer, Transform, AnotherComponent, ...etc>.
	
	INTERFACES AND ABSTRACTION: Especially when your project is larger, you want to make things as abstract as possible.
	You can abstract the implementation of your factories and systems by using interfaces, like this:
	cecsar.Get<IOrcFactory, OrcFactoryImp>.
	You just need to call that once and it will be initialized. Do take note that this system uses lazy initialization,
	and if IOrcFactory is already initialized, the second template parameter won't matter!
	Make sure you define your interfaces before you actually start using the framework.
	 */
	class Cecsar final
	{
	private:
		class Module
		{
		public:
			Cecsar* _cecsar = nullptr;

			virtual ~Module() = default;
			virtual void Initialize(Cecsar& cecsar);
		};

		class InternalSystem : public Module
		{
		public:
			virtual void Update() = 0;
		};

		class InternalFactory : public Module
		{
		public:
			virtual void Construct(int32_t id) = 0;
		};
		
	public:
		struct Info final
		{
			int32_t capacity = 1e3;
		};

		struct Entity final
		{
			int32_t id = -1;
			int32_t index = -1;

			constexpr bool operator==(const Entity& other) const;
		};

		template <typename Component, typename ...Args>
		class System : public InternalSystem
		{
		public:
			constexpr void Update() final override;

		private:
			using Module::_cecsar;
			
			virtual void OnUpdate(utils::SparseSet<Component>& instances, utils::SparseSet<Args>&...) = 0;
		};

		class Factory : public InternalFactory
		{
		public:
			inline void Construct(int32_t id) final override;

		private:
			using Module::_cecsar;
			
			virtual void OnConstruct(Cecsar& cecsar, int32_t id) = 0;
		};

		const Info info;

		inline Cecsar(const Info& info = Info());
		inline ~Cecsar();

		[[nodiscard]] constexpr Entity Spawn();
		inline void Destroy(int32_t index) const;
		[[nodiscard]] constexpr bool Validate(const Entity& entity) const;
		
		template <typename Interface, typename InitType = Interface>
		constexpr Interface& Get();

		template <typename Component>
		constexpr utils::SparseSet<Component>& GetSet();

		[[nodiscard]] constexpr int32_t GetGlobalEntityIndex() const;

	private:
		class AbstractSet : public Module
		{
		public:
			virtual void Remove(int32_t index) = 0;
		};

		template <typename Component>
		class Set final : public AbstractSet
		{
		public:
			utils::SparseSet<Component>* components = nullptr;
			
			~Set();
			void Initialize(Cecsar& cecsar) override;
			
			constexpr void Remove(int32_t index) override;
		};
		
		enum class ModuleType
		{
			System,
			Factory,
			Set,
			Count
		};

		int32_t _globalEntityIndex = 0;
		utils::SparseSet<Entity>* _entities = nullptr;
		std::unordered_map<std::type_index, Module*> _modules[static_cast<int32_t>(ModuleType::Count)]{};

		constexpr static ModuleType Get(InternalSystem* out);
		constexpr static ModuleType Get(InternalFactory* out);
	};

	template <typename Component, typename ... Args>
	constexpr void Cecsar::System<Component, Args...>::Update()
	{
		OnUpdate(_cecsar->GetSet<Component>(), _cecsar->GetSet<Args>()...);
	}

	template <typename Component>
	Cecsar::Set<Component>::Set::~Set()
	{
		delete components;
	}

	template <typename Component>
	void Cecsar::Set<Component>::Initialize(Cecsar& cecsar)
	{
		components = new utils::SparseSet<Component>(cecsar.info.capacity);
	}

	template <typename Interface, typename InitType>
	constexpr Interface& Cecsar::Get()
	{
		Interface* i = nullptr;
		const auto type = Get(i);
		Module*& ptr = _modules[static_cast<int32_t>(type)][typeid(Interface)];
		if(!ptr)
		{
			ptr = new InitType;
			
			ptr->_cecsar = this;
			ptr->Initialize(*this);
		}

		auto* interface = static_cast<Interface*>(ptr);
		return *interface;
	}

	template <typename Component>
	constexpr utils::SparseSet<Component>& Cecsar::GetSet()
	{
		Module*& ptr = _modules[static_cast<int32_t>(ModuleType::Set)][typeid(Component)];
		if(!ptr)
		{
			ptr = new Set<Component>;
			ptr->Initialize(*this);
		}

		auto set = static_cast<Set<Component>*>(ptr);
		return *set->components;
	}

	constexpr bool Cecsar::Entity::operator==(const Entity& other) const
	{
		return index == other.index && id == other.id;
	}

	template <typename Component>
	constexpr void Cecsar::Set<Component>::Remove(const int32_t index)
	{
		components->RemoveAt(index);
	}

	inline void Cecsar::Factory::Construct(int32_t id)
	{
		OnConstruct(*_cecsar, id);
	}

	inline Cecsar::Cecsar(const Info& info) : info(info)
	{
		_entities = new utils::SparseSet<Entity>(info.capacity);
	}

	inline Cecsar::~Cecsar()
	{
		delete _entities;
		for (auto& map : _modules)
			for (const auto& pair : map)
				delete pair.second;
	}

	constexpr Cecsar::ModuleType Cecsar::Get(InternalSystem*)
	{
		return ModuleType::System;
	}

	constexpr Cecsar::ModuleType Cecsar::Get(InternalFactory*)
	{
		return ModuleType::Factory;
	}

	constexpr Cecsar::Entity Cecsar::Spawn()
	{
		const int32_t index = _entities->Add(
			{
				_globalEntityIndex++
			});
		
		auto& entity = _entities->Get(index);
		entity.index = index;
		return entity;
	}

	inline void Cecsar::Destroy(const int32_t index) const
	{
		_entities->RemoveAt(index);

		const auto& sets = _modules[static_cast<int32_t>(ModuleType::Set)];
		for (const auto& pair : sets)
		{
			auto* const set = static_cast<AbstractSet*>(pair.second);
			set->Remove(index);
		}
	}

	inline void Cecsar::Module::Initialize(Cecsar&)
	{
	}

	constexpr bool Cecsar::Validate(const Entity& entity) const
	{
		if (entity.id == -1)
			return false;
		return _entities->Get(entity.index).id == entity.id;
	}

	constexpr int32_t Cecsar::GetGlobalEntityIndex() const
	{
		return _globalEntityIndex;
	}
}
