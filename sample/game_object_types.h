#pragma once

#include "../gamefw/gamefw_game_object.h"
#include "component_types.h"

using SceneChangeManager = gamefw::GameObject<gamefw::NextScene>;
using Player = gamefw::GameObject<PlayerScript>;
using Enemy = gamefw::GameObject<EnemyScript>;
