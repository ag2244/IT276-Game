#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#ifndef __COLLISION_H__
#define __COLLISION_H__

/** @struct Collider_Box
 *  @brief box-shaped collider
 *  @var Collider_Box::width
 *  Member 'width' is float for width (x)
 *  @var Collider_Box::height
 *  Member 'height' is float for height (y)
 */
typedef struct Collider_Box_s 
{

	Vector2D position;
	Vector2D viewpos;

	Vector2D size; //Bottom right corner of the box
	
	Bool(*is_clickable)(struct Collider_Box_s self, Vector2D otherPosition);

} Collider_Box;

/** @struct Collider_Circle
 *  @brief circle-shaped collider
 *  @var Collider_Circle::radius
 *  Member 'radius' is float for distance from x,y position of the circle's edge
 */
typedef struct Collider_Circle_s 
{

	float radius;

	Vector2D position;
	Vector2D viewpos;

	Bool(*is_clickable)(struct Collider_Circle_s self, Vector2D otherPosition);

} Collider_Circle;

/**
* @brief get a new box collider
*/
Collider_Box* box_new();

/**
* @brief get a new circle collider
*/
Collider_Circle* circle_new();

/**
* @brief free requested Collider_Box
* @param box the Collider_Box to free
*/
void box_free(Collider_Box* box);

/**
* @brief free requested Collider_Circle
* @param circle the Collider_Circle to free
*/
void circle_free(Collider_Circle* circle);

/**
* @brief checks if a point is within the specified box
* @param self the box to check with
* @param otherX the x coordinate of the point
* @param otherY the y coordinate of the point
*/
Bool box_clickable(Collider_Box* self, Vector2D otherPosition);

/*
* @brief Checks if two boxes are colliding
* @param box0 The first box
* @param box1 The second box
* @return 1 if colliding, 0 if not
*/
Bool box_detectCollision(Collider_Box* box0, Collider_Box* box1);

/**
* @brief checks if a point is within the specified circle
* @param self the circle to check with
* @param otherX the x coordinate of the point
* @param otherY the y coordinate of the point
*/
Bool circle_clickable(Collider_Circle* self, Vector2D otherPosition);

#endif
