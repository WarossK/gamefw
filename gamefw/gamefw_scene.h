#pragma once

#include <entt/entt.hpp>
#include <memory>
#include <mutex>
#include "gamefw_cond_wait_thread.h"
#include "gamefw_singleton.h"
#include "gamefw_define.h"

namespace gamefw
{
	class Scene : public std::enable_shared_from_this<Scene>
	{
		friend class SceneManager;
	protected:
		std::mutex registry_mutex_;
		Registry registry_;

	public:
		Scene() {}
		virtual ~Scene() {}

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void Uninitialize() = 0;

		virtual bool FadeIn() { return true; }
		virtual bool FadeOut() { return true; }

		void DestroyObjects();
	};

	class SceneManager : public singleton_base<SceneManager>
	{
	private:
		std::shared_ptr<Scene> current_;
		std::shared_ptr<Scene> waiting_;
		std::shared_ptr<Scene> loading_scene_;
		cond_wait_thread load_thread_;
		bool is_load_;

	public:
		SceneManager();
		~SceneManager();
		void Initialize(Scene* scene, Scene* loading_scene);
		void Initialize();
		void Update();
		void Uninitialize();

		void SceneChangeCheck(std::shared_ptr<Scene>& current_scene);
		void SceneLoadCheck();
		void SceneChange();
	};

	struct NextScene
	{
		std::shared_ptr<Scene> next;

		NextScene() {}
		NextScene(std::shared_ptr<Scene> next_scene) : next(next_scene) {}

		virtual ~NextScene() {}
	};
}