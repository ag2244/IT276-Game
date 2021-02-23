#pragma once

#ifndef __STARSABOVE_H__
#define __STARSABOVE_H__

#include "starsabove_entity.h"
#include "starsabove_player.h"

/**
* @brief Spawn a player entity
* @param position The screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity* player_spawn(Vector2D position);

/**
* @brief Do stuff before the game loop begins
* @return NULL on error
*/
void prepare_game();

/**
* @brief The game loop itself, called from game.c
* @return NULL on error
*/
void starsabove_loop();

#endif
