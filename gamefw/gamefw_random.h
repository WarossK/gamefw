#pragma once

#include <random>

//WindowsでGCCでコンパイルする時、シード値が一定になるので注意

namespace gamefw
{
	class high_precision_random
	{
	private:
		std::mt19937 engine_;

	public:
		high_precision_random() : engine_(std::random_device()()) {}
		high_precision_random(std::nullptr_t) : engine_() {}

		template<typename ReturnType>
		ReturnType operator()(
			ReturnType min = std::numeric_limits<ReturnType>::min(),
			ReturnType max = std::numeric_limits<ReturnType>::max())
		{
			static_assert(std::is_arithmetic_v<ReturnType>, "high_precision_random::execute >> not arithmetic value.");

			if constexpr (std::is_integral_v<ReturnType>)
			{
				std::uniform_int_distribution<> dist(min, max);
				return dist(engine_);
			}
			else if constexpr (std::is_floating_point_v<ReturnType>)
			{
				std::uniform_real_distribution<> dist(min, max);
				return dist(engine_);
			}
		}
	};

	class high_speed_random
	{
	private:
		std::minstd_rand engine_;

	public:
		high_speed_random() : engine_(std::random_device()()) {}
		high_speed_random(std::nullptr_t) : engine_() {}

		template<typename ReturnType>
		ReturnType operator()(
			ReturnType min = std::numeric_limits<ReturnType>::min(),
			ReturnType max = std::numeric_limits<ReturnType>::max())
		{
			static_assert(std::is_arithmetic_v<ReturnType>, "high_speed_random::execute >> not arithmetic value.");

			if constexpr (std::is_integral_v<ReturnType>)
			{
				std::uniform_int_distribution dist(min, max);
				return dist(engine_);
			}
			else if constexpr (std::is_floating_point_v<ReturnType>)
			{
				std::uniform_real_distribution dist(min, max);
				return dist(engine_);
			}
		}
	};
}