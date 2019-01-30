#pragma once

#include <Windows.h>
#include <string>
#include <boost/signals2.hpp>
#include "gamefw_singleton.h"

namespace gamefw
{
	class Window : public singleton_base<Window>
	{
	private:
		HWND handle_;
		MSG window_message_;
		std::string app_name_;
		boost::signals2::signal<void(UINT, WPARAM, LPARAM)> callback_;

	public:
		void Initialize(uint32_t width, uint32_t height, std::string app_name);
		bool ProcessMessage();
		void Uninitialize();

		void DoCallback(UINT message, WPARAM wparam, LPARAM lparam);
	};
}