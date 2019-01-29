#pragma once

#include <Windows.h>
#include <string>

namespace gamefw
{
	class Window
	{
	private:
		HWND handle_;
		std::string app_name_;

	public:
		void Initialize(uint32_t width, uint32_t height, std::string app_name);
		bool ProcessMessage();
		void Uninitialize();
	};
}