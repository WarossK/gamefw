#pragma once

#include <chrono>
#include <string>
#include <ctime>

namespace gamefw
{
	class timer
	{
	private:
		std::chrono::time_point<std::chrono::system_clock> posix_time;

	public:
		inline void start()
		{
			posix_time = std::chrono::system_clock::now();
		}

		template<class TimeUnit = std::chrono::milliseconds, class = std::enable_if_t<std::chrono::_Is_duration_v<TimeUnit>>>
		inline int64_t get_elapse()
		{
			return std::chrono::duration_cast<TimeUnit>(std::chrono::system_clock::now() - posix_time).count();
		}

		template<class TimeUnit = std::chrono::milliseconds, class = std::enable_if_t<std::chrono::_Is_duration_v<TimeUnit>>>
		inline int64_t get_elapse_and_reset()
		{
			auto elapse_time = std::chrono::duration_cast<TimeUnit>(std::chrono::system_clock::now() - posix_time).count();
			posix_time = std::chrono::system_clock::now();
			return elapse_time;
		}
	};

	//C++20以降ではカレンダーが追加されるので非推奨。
	//yyyy/mm/dd/hh/mm/ss
	inline std::string get_calender()
	{
		auto now_posix_time = std::chrono::system_clock::now();
		auto c_style_time = std::chrono::system_clock::to_time_t(now_posix_time);
		auto local_time = std::localtime(&c_style_time);

		constexpr char slash[] = "/";
		return {
			std::to_string(local_time->tm_year)
			+ slash
			+ std::to_string(local_time->tm_mon)
			+ slash
			+ std::to_string(local_time->tm_mday)
			+ slash
			+ std::to_string(local_time->tm_hour)
			+ slash
			+ std::to_string(local_time->tm_min)
			+ slash
			+ std::to_string(local_time->tm_sec) };
	}
}