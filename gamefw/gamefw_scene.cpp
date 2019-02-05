#include "gamefw_scene.h"
#include "gamefw_game_object.h"

using namespace gamefw;

gamefw::SceneManager::SceneManager() :
	is_load_(false)
{
}

gamefw::SceneManager::~SceneManager()
{
}

void gamefw::SceneManager::Initialize(Scene* scene, Scene* loading_scene)
{
	current_.reset(scene);
	current_->Initialize();
	loading_scene_.reset(loading_scene);
	loading_scene_->Initialize();
}

void gamefw::SceneManager::Initialize()
{
	current_->Initialize();
}

void gamefw::SceneManager::Update()
{
	if (!is_load_)
	{
		current_->Update();
		current_->Render();
		SceneChangeCheck(current_);
		current_->DestroyObjects();
	}
	else
	{
		loading_scene_->Update();
		loading_scene_->Render();
		SceneLoadCheck();
		loading_scene_->DestroyObjects();
	}
}

void gamefw::SceneManager::Uninitialize()
{
	current_->Uninitialize();
	loading_scene_->Uninitialize();

	if(waiting_) waiting_->Uninitialize();
}

void gamefw::Scene::DestroyObjects()
{
	registry_.destroy<Destroy>();
}

void gamefw::SceneManager::SceneChangeCheck(std::shared_ptr<Scene>& current_scene)
{
	std::shared_ptr<Scene> next;
	auto& registry = current_scene->registry_;
	auto view = registry.view<NextScene>();

	for (const auto entity : view)
	{
		next = view.get(entity).next;
		registry.remove<NextScene>(entity);
	}

	if (next)
	{
		waiting_ = next;
		load_thread_.enqueue([](const std::shared_ptr<Scene>& next)
		{
			next->Initialize();
		}, next);
		is_load_ = true;
	}
}

void gamefw::SceneManager::SceneLoadCheck()
{
	if (load_thread_.is_complete_process())
	{
		is_load_ = false;
		SceneChange();
	}
}

void gamefw::SceneManager::SceneChange()
{
	current_.swap(waiting_);
	waiting_->Uninitialize();
	waiting_.reset();
}
