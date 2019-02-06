#pragma once

#include <entt/entt.hpp>
#include "gamefw_define.h"

namespace gamefw
{
	template<class... Components>
	class GameObject
	{
	private:
		entity_type entity_;

	public:
		GameObject() : entity_(std::numeric_limits<entity_type>::max()) {}
		GameObject(entity_type entity) : entity_(entity) {}

		operator entity_type()
		{
			return entity_;
		}

		GameObject operator = (entity_type entity)
		{
			entity_ = entity;
		}

		static GameObject Create(Registry& registry, Components... components)
		{
			auto entity = registry.create();
			Set(entity, registry, components...);
			return entity;
		}

		static GameObject Create(Registry& registry)
		{
			auto entity = registry.create();
			SetDefault<Components...>(entity, registry);
			return entity;
		}

		template<class Component, class...Args>
		static void AddComponent(entity_type entity, Registry& registry, Args... component_args)
		{
			registry.assign<Component>(entity, component_args...);
		}

		template<class... Components>
		static void AddComponents(entity_type entity, Registry& registry, Components... components)
		{
			Set(entity, registry, components...);
		}

		template<class... Components>
		static void AddComponents(entity_type entity, Registry& registry)
		{
			SetDefault<Components...>(entity, registry);
		}

	private:
		template<class ComponentHead, class... ComponentTail>
		static void Set(const entity_type& entity, Registry& registry, ComponentHead& head, ComponentTail&... tail)
		{
			registry.assign<ComponentHead>(entity, head);

			if constexpr (sizeof...(ComponentTail))
			{
				Set(entity, registry, tail...);
			}
		}
		template<class ComponentHead, class... ComponentTail>
		static void SetDefault(const entity_type& entity, Registry& registry)
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