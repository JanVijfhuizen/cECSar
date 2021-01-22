#pragma once
#include <cstdint>
#include <typeindex>
#include <unordered_map>

#include "SparseSet.h"

namespace revamped
{
	/*
	SUMMARY:
	This is a ECS framework that tries to be as beginner-friendly as possible,
	while still having the option to do some more complex stuff.

	FUNDAMENTALS:
	Classes: Entity, Info,
	Methods: GetSet(), Spawn(), Destroy(), Validate(), Module::Initialize()

	INTERMEDIATE:
	Classes: System, Factory
	Method: Get()

	ADVANCED:
	Method: Get() abstraction.

	MISC:
	Method: GetGlobalEntityIndex()
	 */
	class Cecsar final
	{
		/*
		I'm not going to comment internal workings in detail,
		because the end user won't use it anyway.
		*/
	private:
		/*
		Base module where every cecsar related type inherits from.
		 */
		class Module
		{
		public:
			Cecsar* _cecsar = nullptr;

			virtual ~Module() = default;

			/*
			Used for initialization.
			Empty by default, but you can use it to set up stuff at the start of the cecsar lifespan.
			 */
			virtual void Initialize(Cecsar& cecsar);
		};
		
		// Due to the templated nature of the child class, I have to make a transition class.
		class InternalSystem : public Module
		{
		public:
			virtual void Update() = 0;
		};
		
		// Due to the templated nature of the child class, I have to make a transition class.
		class InternalFactory : public Module
		{
		public:
			virtual void Construct(int32_t id) = 0;
		};
		
	public:
		/*
		Settings for the cecsar.
		Cannot be adjusted after construction.
		 */
		struct Info final
		{
			// The maximum number of entities possible.
			int32_t capacity = 1e3;
		};

		/*
		The entity struct can be used to compare entities, and to access the corresponding components.
		 */
		struct Entity final
		{
			// The ID can be used to compare entities.
			int32_t id = -1;
			/*
			The index can be used to add, remove or adjust components, like so:

			auto entity = cecsar.Spawn();
			cecsar.GetSet<Renderer>().Insert(entity.index)
			 */
			int32_t index = -1;

			constexpr bool operator==(const Entity& other) const;
		};

		/*
		INTERMEDIATE
		
		SYSTEMS are used to define behaviour for a set of components.
		A RenderSystem might manage the Renderer components for instance.
		You can create your custom System by inheriting like this: RenderSystem : public Cecsar::System<Renderer>.
		
		In the missing OnUpdate call, you can define your behaviour, which can be as simple as this:
		foreach(auto& renderer : instances) Render(renderer).
		After that, just call Get<RenderSystem>().Update() to use it.

		Systems do usually manage only one component type, but often these components can be dependent on other components,
		like a Renderer is dependent on the Transform component.
		When defining your system, you can easily get access to these other sets by defining it like this:
		RenderSystem : public Cecsar::System<Renderer, Transform, AnotherComponent, ...etc>.
		 */
		template <typename Component, typename ...Args>
		class System : public InternalSystem
		{
		public:
			/*
			Updates the system, so for a RenderSystem this would mean to visualize all the renderers,
			for a CollisionSystem to check collisions, etc.
			 */
			constexpr void Update() final override;

		private:
			using Module::_cecsar;

			/*
			Called when Update is called.
			 */
			virtual void OnUpdate(utils::SparseSet<Component>& instances, utils::SparseSet<Args>&...) = 0;
		};

		/*
		INTERMEDIATE
		
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
		 */
		class Factory : public InternalFactory
		{
		public:
			/*
			Execute a streamlined construction over an entity.
			This doesn't create an entity, it only expands on it!
			 */
			inline void Construct(int32_t id) final override;

		private:
			using Module::_cecsar;

			/*
			Called when Construct is called.
			 */
			virtual void OnConstruct(Cecsar& cecsar, int32_t id) = 0;
		};

		// Information regarding the current instance of cecsar.
		const Info info;

		inline Cecsar(const Info& info = Info());
		inline ~Cecsar();

		/*
		Spawns an entity. Returns an Entity struct, which provides the data you need to
		further define the entity.
		 */
		[[nodiscard]] constexpr Entity Spawn();

		// Destroys an entity. Takes in the entity's index.
		inline void Destroy(int32_t index) const;

		/*
		Validate an entity.
		An entity's index might be the same as some earlier destroyed entity,
		but the ID's are always unique.
		*/	
		[[nodiscard]] constexpr bool Validate(const Entity& entity) const;

		/*
		INTERMEDIATE:
		
		Use this method to get systems and factories, like so:
		Get<RenderSystem>, or Get<OrcFactory>.

		ADVANCED:
		
		Interfaces and abstraction:
		Especially when your project is larger, you want to make things as abstract as possible.
		You can abstract the implementation of your factories and systems by using interfaces, like this:
		cecsar.Get<IOrcFactory, OrcFactoryImp>.
		You just need to call that once and it will be initialized.
		
		Do take note that this system uses lazy initialization,
		and if IOrcFactory is already initialized, the second template parameter won't matter!
		Make sure you define your interfaces before you actually start using the framework.
		*/
		template <typename Interface, typename InitType = Interface>
		constexpr Interface& Get();

		/*
		Gets the set of a target component.
		You might want to read up on sparse set, as they're not often used.
		The way they can be used is as follows:

		you can iterate over them with a foreach loop: foreach(auto& component : set).
		you can use a for loop: for(int i = 0; i < set.GetCount(); ++i).

		You have functionality like Insert, RemoveAt and Get.

		While the iterating is done in a unordered way,
		you can get the corresponding indexes for a component by using the dense set.
		This is useful if you want to get to other component the entity might have.
		
		const auto dense = set.GetDenseRaw();
		for(int i = 0; i < set.GetCount(); ++i){
			auto& component = set[i];
			
			const int index = dense[i];
			auto& otherComponent = otherSet.Get(index);
		}
		 */
		template <typename Component>
		constexpr utils::SparseSet<Component>& GetSet();

		/*
		
		*/
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
