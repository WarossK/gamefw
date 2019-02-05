#include "gamefw_engine.h"

gamefw::Engine::Engine()
{
}

gamefw::Engine::~Engine()
{
}

void gamefw::Engine::Run()
{
	while (window_->ProcessMessage())
	{
		scene_manager_->Update();
	}
}

void gamefw::Engine::Uninitialize()
{
	scene_manager_->Uninitialize();
	window_->Uninitialize();
}