#pragma once
#include <cstdint>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

#include <SparseSet.h>

namespace revamped
{
	/*
	This is an ECS framework that tries to be as beginner-friendly as possible,
	while still having the option to do some more complex things with it.
	 */
	class Cecsar final
	{
#pragma region Custom Data Types
		/*
		I'm not going to comment internal workings in detail,
		because the end user wont use it anyway.
		*/
	public:
		// ECS settings used by cecsar.
		struct Info final
		{
			// The maximum number of entities possible.		
			int32_t capacity = 1e3;
			// The next entity spawns with this ID, after which this increments.
			int32_t globalEntityStartIndex = 0;
		};

		// Class that stores entity information.
		struct Entity final
		{
			// Can be used to compare entities.
			int32_t id = -1;
			// Can be used to search up components.
			int32_t index = -1;

			constexpr bool operator==(const Entity& other) const;
		};
	
	private:
		// Base module where every cecsar related type inherits from.
		class Module
		{
		public:
			Cecsar* _cecsar = nullptr;

			virtual ~Module() = default;

			// Used for initialization.
			virtual void Initialize(Cecsar& cecsar);
		};

		// Due to the templated nature of some classes, I have to make intermediate base classes for them.
#pragma region Intermediate Classes
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
		
		class InternalSet : public Module
		{
		public:
			virtual void Remove(int32_t index) = 0;
		};
#pragma endregion

		// Internal tools to make lookup faster and decrease boilerplate.
#pragma region Internal Enums
		enum class ModuleType
		{
			System,
			Factory,
			Count
		};

		enum class SetType
		{
			Hot,
			Cold,
			Map,
			Count
		};
#pragma endregion 

	public:
#pragma region Optional Tools
		// Can be used to define behaviour for one or more components.
		template <typename Component, typename ...Args>
		class System : public InternalSystem
		{
		public:
			constexpr void Update() final override;

		private:
			using Module::_cecsar;

			// Called after Cecsar::Update is called.
			virtual void OnUpdate(utils::SparseSet<Component>& instances, utils::SparseSet<Args>&...) = 0;
		};

		// Can be used to streamline entity construction.
		class Factory : public InternalFactory
		{
		public:
			// Construct over target entity.
			inline void Construct(int32_t id) final override;

		private:
			using Module::_cecsar;

			// Called after Construct is called.
			virtual void OnConstruct(Cecsar& cecsar, int32_t id) = 0;
		};
#pragma endregion

#pragma region Sets
		// Set that contains a sparse set for the corresponding component type.
		template <typename Component>
		class DefaultSet final : public InternalSet
		{
		public:
			utils::SparseSet<Component>* components = nullptr;

			// Delete set.
			~DefaultSet();
			// Initialize set.
			void Initialize(Cecsar& cecsar) override;
			// Remove at index.
			constexpr void Remove(int32_t index) override;
		};

		// Set that contains a map for the corresponding component type.
		template <typename Component>
		class MapSet : public InternalSet
		{
		public:
			std::unordered_map<int32_t, Component> components{};

			// Remove at index.
			constexpr void Remove(int32_t index) override;
		};
		
		// Set that contains a map for the corresponding component type.
		template <typename ...Args>
		class SoASet : public InternalSet
		{
		public:
			~SoASet();

			// Get entity at target index.
			template <size_t S>
			constexpr auto Get();

			// Initialize set.
			void Initialize(Cecsar& cecsar) override;
			// Remove at index.
			constexpr void Remove(int32_t index) final override;

		private:
			std::tuple<Args*...> _args{};

			// Initializes all member arrays.
			template <size_t S>
			constexpr void InitMembers(int32_t capacity);

			template <typename T>
			static constexpr void InitMember(T*& member, int32_t capacity);

			// Delete all member arrays.
			template <size_t S>
			constexpr void DeleteMembers();

			template <typename T>
			static constexpr void DeleteMember(T*& member);

			// Clears target entity.
			template <size_t S>
			constexpr void ClearMembers(int32_t index);

			template <typename T>
			static constexpr void ClearMember(T*& member, int32_t index);
		};
#pragma endregion
#pragma endregion

#pragma region Variables
		// Information regarding the current instance of cecsar.
		const Info info;

	private:
		// Global index, given to new entities as their id, and then incremented.
		int32_t _globalEntityIndex = 0;
		utils::SparseSet<Entity>* _entities = nullptr;

		// Contains the systems and factories.
		std::unordered_map<std::type_index, Module*> _modules[static_cast<int32_t>(ModuleType::Count)]{};
		// Contains the various sets.
		std::unordered_map<std::type_index, Module*> _sets[static_cast<int32_t>(SetType::Count)]{};
#pragma endregion 

#pragma region Methods
	public:
		inline Cecsar(const Info& info = Info());
		inline ~Cecsar();

		// Spawns an entity and returns that entity's info.
		[[nodiscard]] constexpr Entity Spawn();

		// Destroys an entity.
		inline void Destroy(int32_t index) const;

		// Checks if the entity exists.
		[[nodiscard]] constexpr bool Validate(const Entity& entity) const;

		/*
		Get target System or Factory.
		Define InitType to set the specific implementation (Interface will be chosen by default.)
		*/
		template <typename Interface, typename InitType = Interface>
		constexpr Interface& Get();

		// Get the target component set. Different types of sets are not interchangeable.
		template <typename Component>
		constexpr utils::SparseSet<Component>& GetSet();

		// Get the target component set. Different types of sets are not interchangeable.
		template <typename Component>
		constexpr std::unordered_map<int32_t, Component>& GetMapSet();

		// Get the target component set. Different types of sets are not interchangeable.
		template <typename ComponentSoA>
		constexpr ComponentSoA& GetColdSet();

		// The next entity spawns with this ID, after which this increments.
		[[nodiscard]] constexpr int32_t GetGlobalEntityIndex() const;

		// Get the current entity count.
		[[nodiscard]] constexpr int32_t GetCount() const;

	private:
		// Used to make the type check constexpr.
		constexpr static ModuleType Get(InternalSystem* out);
		// Used to make the type check constexpr.
		constexpr static ModuleType Get(InternalFactory* out);
#pragma endregion 
	};

	template <typename Component, typename ... Args>
	constexpr void Cecsar::System<Component, Args...>::Update()
	{
		OnUpdate(_cecsar->GetSet<Component>(), _cecsar->GetSet<Args>()...);
	}

	template <typename ... Args>
	template <size_t S>
	constexpr auto Cecsar::SoASet<Args...>::Get()
	{
		return std::get<S>(_args);
	}

	template <typename ... Args>
	constexpr void Cecsar::SoASet<Args...>::Remove(int32_t index)
	{
		ClearMembers<sizeof...(Args) - 1>(index);
	}

	template <typename ... Args>
	template <size_t S>
	constexpr void Cecsar::SoASet<Args...>::InitMembers(const int32_t capacity)
	{
		InitMember(std::get<S>(_args), capacity);
		if constexpr (S > 0)
			InitMembers<S - 1>(capacity);
	}

	template <typename ... Args>
	template <typename T>
	constexpr void Cecsar::SoASet<Args...>::InitMember(T*& member, const int32_t capacity)
	{
		member = new T[capacity];
	}

	template <typename ... Args>
	template <size_t S>
	constexpr void Cecsar::SoASet<Args...>::DeleteMembers()
	{
		DeleteMember(std::get<S>(_args));
		if constexpr (S > 0)
			DeleteMembers<S - 1>();
	}

	template <typename ... Args>
	template <typename T>
	constexpr void Cecsar::SoASet<Args...>::DeleteMember(T*& member)
	{
		delete[] member;
	}

	template <typename ... Args>
	template <size_t S>
	constexpr void Cecsar::SoASet<Args...>::ClearMembers(const int32_t index)
	{
		ClearMember(std::get<S>(_args), index);
		if constexpr (S > 0)
			ClearMembers<S - 1>(index);
	}

	template <typename ... Args>
	template <typename T>
	constexpr void Cecsar::SoASet<Args...>::ClearMember(T*& member, const int32_t index)
	{
		member[index] = T();
	}

	template <typename ... Args>
	Cecsar::SoASet<Args...>::~SoASet()
	{
		DeleteMembers<sizeof...(Args) - 1>();
	}

	template <typename ... Args>
	void Cecsar::SoASet<Args...>::Initialize(Cecsar& cecsar)
	{
		InitMembers<sizeof...(Args) - 1>(cecsar.info.capacity);
	}

	template <typename Component>
	Cecsar::DefaultSet<Component>::DefaultSet::~DefaultSet()
	{
		delete components;
	}

	template <typename Component>
	void Cecsar::DefaultSet<Component>::Initialize(Cecsar& cecsar)
	{
		components = new utils::SparseSet<Component>(cecsar.info.capacity);
	}

	template <typename Interface, typename InitType>
	constexpr Interface& Cecsar::Get()
	{
		Interface* i = nullptr;
		const auto type = Get(i);

		Module*& ptr = _modules[static_cast<int32_t>(type)][typeid(Interface)];
		if (!ptr)
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
		Module*& ptr = _sets[static_cast<int32_t>(SetType::Hot)][typeid(Component)];
		if (!ptr)
		{
			ptr = new DefaultSet<Component>;
			ptr->Initialize(*this);
		}
		
		auto set = static_cast<DefaultSet<Component>*>(ptr);
		return *set->components;
	}

	template <typename Component>
	constexpr std::unordered_map<int32_t, Component>& Cecsar::GetMapSet()
	{
		Module*& ptr = _sets[static_cast<int32_t>(SetType::Map)][typeid(Component)];
		if (!ptr)
		{
			ptr = new MapSet<Component>;
			ptr->Initialize(*this);
		}

		auto set = static_cast<MapSet<Component>*>(ptr);
		return set->components;
	}

	template <typename ComponentSoA>
	constexpr ComponentSoA& Cecsar::GetColdSet()
	{
		Module*& ptr = _sets[static_cast<int32_t>(SetType::Cold)][typeid(ComponentSoA)];
		if (!ptr)
		{
			ptr = static_cast<Module*>(new ComponentSoA());
			ptr->Initialize(*this);
		}

		auto set = static_cast<ComponentSoA*>(ptr);
		return *set;
	}

	constexpr bool Cecsar::Entity::operator==(const Entity& other) const
	{
		return index == other.index && id == other.id;
	}

	template <typename Component>
	constexpr void Cecsar::DefaultSet<Component>::Remove(const int32_t index)
	{
		components->RemoveAt(index);
	}

	template <typename Component>
	constexpr void Cecsar::MapSet<Component>::Remove(const int32_t index)
	{
		
	}

	inline void Cecsar::Factory::Construct(const int32_t id)
	{
		OnConstruct(*_cecsar, id);
	}

	inline Cecsar::Cecsar(const Info& info) :
		info(info), _globalEntityIndex(info.globalEntityStartIndex)
	{
		_entities = new utils::SparseSet<Entity>(info.capacity);
	}

	inline Cecsar::~Cecsar()
	{
		delete _entities;
		for (auto& map : _modules)
			for (const auto& pair : map)
				delete pair.second;
		for (auto& map : _sets)
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

		for (int i = static_cast<int32_t>(SetType::Count) - 1; i >= 0; --i)
		{
			const auto& sets = _sets[i];
			for (const auto& pair : sets)
			{
				auto* const set = static_cast<InternalSet*>(pair.second);
				set->Remove(index);
			}
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

	constexpr int32_t Cecsar::GetCount() const
	{
		return _entities->GetCount();
	}
}
