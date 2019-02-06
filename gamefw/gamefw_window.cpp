#include "gamefw_window.h"
#include "gamefw_exception.h"

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

gamefw::Window::Window()
{
	SecureZeroMemory(&window_message_, sizeof(window_message_));
}

gamefw::Window::~Window()
{
	Uninitialize();
}

void gamefw::Window::Initialize(uint32_t width, uint32_t height, std::string app_name)
{
#if defined(GAMEFW_NO_CONSOLE_WINDOW)
	HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	WNDCLASSEX window_class;

	app_name_ = app_name;
	const auto& app_name_str = app_name_.c_str();

	auto instance = GetModuleHandle(nullptr);

	window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window_class.lpfnWndProc = WindowProc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = instance;
	window_class.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	window_class.hIconSm = window_class.hIcon;
	window_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window_class.lpszMenuName = nullptr;
	window_class.lpszClassName = app_name_str;
	window_class.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&window_class))
	{
		GAMEFW_THROW_EXCEPTION("window register failed.");
	}

	int window_style = WS_OVERLAPPEDWINDOW ^ (WS_MAXIMIZEBOX | WS_THICKFRAME);

	RECT window_rectangle = { 0, 0, static_cast<long>(width), static_cast<long>(height) };
	AdjustWindowRect(&window_rectangle, WS_OVERLAPPEDWINDOW, false);
	int screen_width = (SHORT)(window_rectangle.right - window_rectangle.left);
	int screen_height = (SHORT)(window_rectangle.bottom - window_rectangle.top);

	RECT desktop_rectangle;
	GetWindowRect(GetDesktopWindow(), &desktop_rectangle);
	int window_position_x = (desktop_rectangle.right - screen_width) / 2;
	if (window_position_x < 0) window_position_x = 0;
	int window_position_y = (desktop_rectangle.bottom - screen_height) / 2;
	if (window_position_y < 0) window_position_y = 0;

	auto window_handle = CreateWindowEx(
		WS_EX_APPWINDOW,
		app_name_str, app_name_str,
		window_style,
		window_position_x, window_position_y,
		screen_width, screen_height,
		nullptr, nullptr,
		instance, nullptr);

	if (!window_handle)
	{
		PostQuitMessage(0);
		GAMEFW_THROW_EXCEPTION("window create failure.");
	}

	ShowWindow(window_handle, SW_SHOW);
	SetForegroundWindow(window_handle);
	SetFocus(window_handle);

	handle_ = window_handle;
}

bool gamefw::Window::ProcessMessage()
{
	while (PeekMessage(&window_message_, nullptr, 0u, 0u, PM_REMOVE))
	{
		TranslateMessage(&window_message_);
		DispatchMessage(&window_message_);

		if (window_message_.message == WM_QUIT) return false;
	}

	return true;
}

void gamefw::Window::Uninitialize()
{
	if (!handle_) return;
	PostQuitMessage(0);
	DestroyWindow(handle_);
	handle_ = nullptr;
}

void gamefw::Window::DoCallback(UINT message, WPARAM wparam, LPARAM lparam)
{
	callback_(message, wparam, lparam);
}
