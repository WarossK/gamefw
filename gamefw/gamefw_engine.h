#pragma once

#define NOMINMAX
#include <memory>
#include "gamefw_window.h"
#include "gamefw_scene.h"

namespace gamefw
{
	class Engine : public singleton_base<Engine>
	{
	private:
		std::unique_ptr<Window> window_;
		std::shared_ptr<SceneManager> scene_manager_;

	public:
		Engine();
		~Engine();

		template<class InitialScene, class LoadingScene>
		void Initialize(uint32_t width, uint32_t height, std::string app_name)
		{
			window_ = std::make_unique<Window>();
			window_->Initialize(width, height, app_name);

			scene_manager_ = std::make_unique<SceneManager>();
			scene_manager_->Initialize(new InitialScene, new LoadingScene);

#if !defined(GAMEFW_GRAPHIC_API_NULL)
			gamegl::Initialize();
#endif
		}

		void Run();
		void Uninitialize();
	};
}