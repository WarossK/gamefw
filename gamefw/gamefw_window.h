#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>
#include <boost/signals2.hpp>
#include "gamefw_singleton.h"

namespace gamefw
{
	class Window : public singleton_base_2<Window>
	{
	private:
		HWND handle_;
		MSG window_message_;
		std::string app_name_;
		boost::signals2::signal<void(UINT, WPARAM, LPARAM)> callback_;

	public:
		Window();
		~Window();

		void Initialize(uint32_t width, uint32_t height, std::string app_name);
		bool ProcessMessage();
		void Uninitialize();

		void DoCallback(UINT message, WPARAM wparam, LPARAM lparam);
		
		template<class CallbackFunc>
		static void AddCallback(uint32_t call_priority, CallbackFunc callback)
		{
			Window::get_instance()->callback_.connect(call_priority, callback);
		}
	};
}