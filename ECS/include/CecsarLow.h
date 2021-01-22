#pragma once
#include <cstdint>
#include <typeindex>
#include <unordered_map>

#include "SparseSet.h"

namespace revamped
{
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
