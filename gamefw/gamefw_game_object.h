#pragma once

#include <entt/entt.hpp>

namespace gamefw
{
	using entity_type = typename entt::DefaultRegistry::entity_type;

	template<class... Components>
	class GameObject
	{
	public:
		static entity_type Create(entt::DefaultRegistry& registry, Components... components)
		{
			auto entity = registry.create();
			Set(entity, registry, components...);
			return entity;
		}

		static entity_type Create(entt::DefaultRegistry& registry)
		{
			auto entity = registry.create();
			SetDefault<Components...>(entity, registry);
			return entity;
		}

		template<class Component, class...Args>
		static void AddComponent(entity_type entity, entt::DefaultRegistry& registry, Args... component_args)
		{
			registry.assign<Component>(entity, component_args...);
		}

		template<class... Components>
		static void AddComponents(entity_type entity, entt::DefaultRegistry& registry, Components... components)
		{
			Set(entity, registry, components...);
		}

		template<class... Components>
		static void AddComponents(entity_type entity, entt::DefaultRegistry& registry)
		{
			SetDefault<Components...>(entity, registry);
		}

	private:
		template<class ComponentHead, class... ComponentTail>
		static void Set(const typename entt::DefaultRegistry::entity_type& entity, entt::DefaultRegistry& registry, ComponentHead& head, ComponentTail&... tail)
		{
			registry.assign<ComponentHead>(entity, head);

			if constexpr (sizeof...(ComponentTail))
			{
				Set(entity, registry, tail...);
			}
		}
		template<class ComponentHead, class... ComponentTail>
		static void SetDefault(const typename entt::DefaultRegistry::entity_type& entity, entt::DefaultRegistry& registry)
		{
			registry.assign<ComponentHead>(entity, ComponentHead());

			if constexpr (sizeof...(ComponentTail))
			{
				SetDefault<ComponentTail...>(entity, registry);
			}
		}
	};

	struct Destroy {};
	struct Draw3D {};
	struct Draw2D {};
}