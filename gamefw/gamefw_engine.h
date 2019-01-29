#pragma once

#include <memory>
#include "gamefw_window.h"
#include "gamefw_scene.h"

namespace gamefw
{
	class Engine
	{
	private:
		std::unique_ptr<Window> window_;
		std::shared_ptr<SceneManager> scene_manager_;

	public:
		Engine();
		~Engine();

		template<class InitialScene>
		void Initialize(uint32_t width, uint32_t height, std::string app_name)
		{
			window_ = std::make_unique<Window>();
			window_->Initialize(width, height, app_name);

			scene_manager_ = std::make_unique<SceneManager>();
			scene_manager_->Initialize(new InitialScene);
		}

		void Update();
		void Uninitialize();
	};
}