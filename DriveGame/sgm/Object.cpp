#include "Object.h"
#include "GameState.h"

int SGM2::Object::nextId = Object::FIRST_OBJECT_ID;

int SGM2::Object::get_game_state_id() const { return gameState->get_id(); }
