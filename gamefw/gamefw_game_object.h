#pragma once

#include <entt/entt.hpp>

namespace gamefw
{
	template<class... Components>
	class GameObject
	{
	public:
		static void Create(entt::DefaultRegistry& registry, Components... components)
		{
			auto entity = registry.create();
			Set(entity, registry, components...);
		}

	private:
		static void Set(const typename entt::DefaultRegistry::entity_type& entity, entt::DefaultRegistry& registry) {}

		template<class ComponentHead, class... ComponentTail>
		static void Set(const typename entt::DefaultRegistry::entity_type& entity, entt::DefaultRegistry& registry, ComponentHead& head, ComponentTail&... tail)
		{
			registry.assign<ComponentHead>(entity, head);
			Set(entity, registry, tail...);
		}
	};

	struct Destroy{};
	struct Draw3D{};
	struct Draw2D{};
}