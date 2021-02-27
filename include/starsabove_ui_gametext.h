#pragma once

#ifndef __UI_GAMETEXT_H__
#define __UI_GAMETEXT_H__

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"

/**
* @brief Spawn a player entity
* @param position The screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
UI_Element* gametext_init(Vector2D position);

#endif
