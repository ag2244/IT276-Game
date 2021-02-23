#pragma once

#ifndef __CLICK_DETECT_H__
#define __CLICK_DETECT_H__

#include <stdlib.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "starsabove_entity.h"

/**
* @brief Spawn a player entity
* @param position The screen position to spawn the player at
* @return NULL on error, or a pointer to a new player entity
*/
Entity* hovering_over(Vector2D position);

#endif