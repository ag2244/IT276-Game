#pragma once

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "starsabove_entity.h"

/**
* @brief Spawn a star system entity
* @param position The screen position to spawn the system at
* @return NULL on error, or a pointer to a new system entity
*/
Entity* system_spawn(Vector2D position);

#endif
