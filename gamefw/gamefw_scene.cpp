#include "gamefw_scene.h"
#include "gamefw_game_object.h"

using namespace gamefw;

gamefw::SceneManager::SceneManager()
{
}

gamefw::SceneManager::~SceneManager()
{
}

void gamefw::SceneManager::Initialize(Scene* scene)
{
	current_.reset(scene);
	current_->Initialize();
}

void gamefw::SceneManager::Initialize()
{
	current_->Initialize();
}

void gamefw::SceneManager::Update()
{
	current_->Update();
	current_->Render();
	waiting_ = current_->SceneChangeCheck();
	current_->DestroyObjects();
}

void gamefw::SceneManager::Uninitialize()
{
	current_->Uninitialize();
}

void gamefw::SceneManager::AsyncLoadStart()
{
	assert(waiting_);
	auto& waiting = waiting_;
	load_thread_.Enqueue([&waiting]()
	{
		waiting->Initialize();
	});
}

void gamefw::Scene::DestroyObjects()
{
	registry_.destroy<Destroy>();
}

std::shared_ptr<Scene> gamefw::Scene::SceneChangeCheck()
{
	return SceneChange::GetNextScene(registry_);
}
