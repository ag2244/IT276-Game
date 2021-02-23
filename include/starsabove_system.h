#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"

/**
* @brief Spawn a player entity
* @param position The screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity* system_spawn(Vector2D position);

#endif
