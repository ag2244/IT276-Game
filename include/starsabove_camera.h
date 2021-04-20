#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "starsabove_collision.h"

typedef struct
{

	Vector2D pos;
	Vector2D size;

	Vector2D movementspeed;

	Collider_Box* cam_collider;

} Camera;

Camera* camera_get();

Camera* camera_init(Vector2D pos, Vector2D size, Vector2D movementspeed);

Bool camera_inview(Collider_Circle* circle, Collider_Box* box);

void camera_move(Vector2D movement_dir);

void camera_free();

#endif