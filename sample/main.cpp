#define GAMEFW_GRAPHIC_API_NULL
#include "../gamefw/gamefw_engine.h"
#include "../gamefw/gamefw_game_object.h"
#include <iostream>

template<class...Args>
void log(Args... args)
{
	(std::cout << ... << args) << std::endl;
}

using SceneChangeManager = gamefw::GameObject<gamefw::NextScene>;

struct next_scene : public gamefw::Scene
{
	gamefw::entity_type entity;

	void Initialize()
	{
		entity = SceneChangeManager::Create(registry_);
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
	gamefw::entity_type entity;

	void Initialize()
	{
		entity = SceneChangeManager::Create(registry_);
		log("test_scene", " : ", "initialize.");
	}

	void Update()
	{
		if (GetKeyState(VK_SPACE) & 0x80)
		{
			SceneChangeManager::AddComponent<gamefw::NextScene>(entity, registry_, std::make_shared<next_scene>());
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
	if (GetKeyState(VK_SPACE) & 0x80)
	{
		SceneChangeManager::AddComponents(entity, registry_, gamefw::NextScene(std::make_shared<test_scene>()));
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
