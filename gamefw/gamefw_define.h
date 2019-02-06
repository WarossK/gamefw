#pragma once

#include <entt/entt.hpp>

namespace gamefw
{
#if defined(GAMEFW_ENTITY_LIMIT_HIGH)
	using Registry = entt::Registry<uint64_t>;
#elif defined(GAMEFW_ENTITY_LIMIT_LOW)
	using Registry = entt::Registry<uint16_t>;
#else// GAMEFW_ENTITY_LIMIT_DEFAULT
	using Registry = entt::DefaultRegistry;
#endif

using entity_type = typename Registry::entity_type;
}