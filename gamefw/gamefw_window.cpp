#include "gamefw_window.h"

LRESULT __stdcall WindowProc(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		gamefw::Window::get_instance()->DoCallback(message, wparam, lparam);
	}

	return DefWindowProc(window_handle, message, wparam, lparam);
}

void gamefw::Window::Initialize(uint32_t width, uint32_t height, std::string app_name)
{
}

bool gamefw::Window::ProcessMessage()
{
}

void gamefw::Window::Uninitialize()
{
}

void gamefw::Window::DoCallback(UINT message, WPARAM wparam, LPARAM lparam)
{
	callback_(message, wparam, lparam);
}
