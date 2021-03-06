#include "gamefw_scene.h"
#include "gamefw_game_object.h"

using namespace gamefw;

gamefw::SceneManager::SceneManager() :
	state_(UPDATE)
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
	switch (state_)
	{
	case UPDATE:
	{
		current_->Update();
		current_->Render();
		SceneChangeCheck(current_);
		current_->DestroyObjects();
	}
	break;
	case LOADING:
	{
		loading_scene_->Update();
		loading_scene_->Render();
		SceneLoadCheck();
		loading_scene_->DestroyObjects();
	}
	break;
	case FADEIN:
	{
		if (current_->FadeIn())
		{
			state_ = UPDATE;
		}
	}
	break;
	case FADEOUT:
	{
		if (current_->FadeOut())
		{
			state_ = LOADING;
		}
	}
	break;
	}
}

void gamefw::SceneManager::Uninitialize()
{
	current_->Uninitialize();
	loading_scene_->Uninitialize();

	if (waiting_) waiting_->Uninitialize();
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

	for (auto entity : view)
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
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1s);
		}, waiting_);
		state_ = FADEOUT;
	}
}

void gamefw::SceneManager::SceneLoadCheck()
{
	if (load_thread_.is_complete_process())
	{
		SceneChange();
		//current_->Render();
		state_ = FADEIN;
	}
}

void gamefw::SceneManager::SceneChange()
{
	current_.swap(waiting_);
	waiting_->Uninitialize();
	waiting_.reset();
}