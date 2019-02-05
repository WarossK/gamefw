#pragma once

#include <stdexcept>
#include <cassert>

#if defined(_DEBUG) || defined(DEBUG)
#define GAMEFW_THROW_EXCEPTION(x) throw std::runtime_error(x);
#else
#define GAMEFW_THROW_EXCEPTION(x) 0
#endif

#define GAMEFW_ASSERT(expr, msg) _ASSERT_EXPR(expr, msg);

#define GAMEFW_NOEXCEPT noexcept