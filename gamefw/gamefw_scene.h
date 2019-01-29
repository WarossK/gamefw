#pragma once

#include <entt/entt.hpp>
#include <memory>
#include "gamefw_cond_wait_thread.h"

namespace gamefw
{
	class Scene : public std::enable_shared_from_this<Scene>
	{
	private:
		entt::DefaultRegistry registry_;

	public:
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Uninitialize() = 0;

		void DestroyObjects();
		std::shared_ptr<Scene> SceneChangeCheck();
	};

	class SceneManager : public std::enable_shared_from_this<SceneManager>
	{
	private:
		std::shared_ptr<Scene> current_;
		std::shared_ptr<Scene> waiting_;
		cond_wait_thread load_thread_;

		std::thread load_thread_;

	public:
		SceneManager();
		~SceneManager();
		void Initialize(Scene* scene);
		void Initialize();
		void Update();
		void Uninitialize();
		
		//”ñ“¯Šúƒ[ƒh
		void AsyncLoadStart();
	};

	struct SceneChange
	{
		std::shared_ptr<Scene> next;

		static std::shared_ptr<Scene> GetNextScene(entt::DefaultRegistry& registry)
		{
			auto view = registry.view<SceneChange>();
			for (const auto entity : view)
			{
				return view.get(entity).next;
			}

			return nullptr;
		}
	};
}