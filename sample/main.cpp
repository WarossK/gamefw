#include <iostream>
#define GAMEFW_GRAPHIC_API_NULL
#include "../gamefw/gamefw_engine.h"
#include "../gamefw/gamefw_game_object.h"
#include "../gamefw/gamefw_random.h"
#include "game_object_types.h"

//#define GAMEFW_NO_CONSOLE_WINDOW

template<class... Args>
void log(Args... args)
{
	(std::cout << ... << args) << std::endl;
}

struct next_scene : public gamefw::Scene
{
	SceneChangeManager scene_change_manager;

	void Initialize()
	{
		scene_change_manager = SceneChangeManager::Create(registry_);
		log("next_scene", " : ", "initialize.");
	}

	void Update();

	void Render()
	{
		log("next_scene", " : ", "render.");
	}

	void Uninitialize()
	{
		log("next_scene", " : ", "uninitialize.");
	}
};

struct test_scene : public gamefw::Scene
{
	SceneChangeManager scene_change_manager;

	void Initialize()
	{
		gamefw::high_speed_random rand;
		scene_change_manager = SceneChangeManager::Create(registry_);
		Player::Create(registry_, PlayerScript{ rand(0.01f, 0.1f), rand(0.01f, 0.1f), rand(0.01f, 0.1f) });
		Enemy::Create(registry_, EnemyScript{ gamefw::high_speed_random(), rand(0.01f, 0.1f), rand(0.01f, 0.1f), rand(0.01f, 0.1f) });
		log("test_scene", " : ", "initialize.");
	}

	void Update()
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			SceneChangeManager::AddComponent<gamefw::NextScene>(scene_change_manager, registry_, std::make_shared<next_scene>());
		}

		log("test_scene", " : ", "update.");
	}

	void Render()
	{
		log("test_scene", " : ", "render.");
	}

	void Uninitialize()
	{
		log("test_scene", " : ", "uninitialize.");
	}
};

struct test_load_scene : public gamefw::Scene
{
	void Initialize()
	{
		log("test_load_scene : initialize.");
	}

	void Update()
	{
		log("test_load_scene", " : ", "update.");
	}

	void Render()
	{
		log("test_load_scene", " : ", "render.");
	}

	void Uninitialize()
	{
		log("test_load_scene", " : ", "uninitialize.");
	}
};

void next_scene::Update()
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		SceneChangeManager::AddComponents(scene_change_manager, registry_, gamefw::NextScene(std::make_shared<test_scene>()));
	}

	log("next_scene", " : ", "update.");
}

#if !defined(GAMEFW_GRAPHIC_API_NULL) && defined(GAMEFW_NO_CONSOLE_WINDOW)
int __stdcall WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
#else
int main(int args, char* argv[])
#endif
{
	auto engine = std::make_unique<gamefw::Engine>();

	try
	{
		engine->Initialize<test_scene, test_load_scene>(128, 128, "sample");
		engine->Run();
		engine->Uninitialize();
	}
	catch (const std::runtime_error& re)
	{
		MessageBox(nullptr, re.what(), "runtime error.", MB_OK);
		return -1;
	}
}
