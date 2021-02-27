#pragma once

#include <SDL.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

#include "starsabove_collision.h"

#ifndef __UI_H__
#define __UI_H__

/** @struct Collider_Box
 *  @brief box-shaped collider
 *  @var Collider_Box::width
 *  Member 'width' is float for width (x)
 *  @var Collider_Box::height
 *  Member 'height' is float for height (y)
 */
typedef struct UI_Element_s
{

	Bool _inuse;
	Vector2D position;
	Sprite* sprite;

	float frame;
	float frameRate;
	int frameCount;

	/*Function pointers!*/
	void (*update)(struct Entity_s* self);
	void (*think)(struct Entity_s* self);
	void (*draw)(struct Entity_s* self);
	void (*free)(struct Entity_s* self);

	void (*onClick)(struct Entity_s* self);

	void* data;

	Collider_Circle* collider_circle;
	Collider_Box* collider_box;

} UI_Element;

typedef struct
{
	UI_Element* element_list;
	Uint32 max_elements;

} UI_Manager;

/**
* @brief initialize internal ui element management system
* @param max_elements how many concurrent ui elements to support
*/
void ui_manager_init(Uint32 max_elements);

/**
* @brief gets the active ui manager
* @return the active ui manager
*/
UI_Manager* ui_manager_get();

/**
* @brief calls update function on all ui elements
*/
void ui_manager_update();

/**
* @brief calls draw function on all ui elements
*/
void ui_manager_draw();

/**
* @brief free all ui elements in the system, destroy ui manager
*/
void ui_manager_free();

/**
* @brief allocate a ui element, initialize to zero, return a pointer to it
* @return NULL on error (see logs) or a pointer to an initialized ui element
*/
UI_Element* ui_new();

/**
* @brief free requested entity
* @param element the entity to free
*/
void ui_free(UI_Element* element);

/**
* @brief draw an ui element to the current render frame
* @param element the ui element to draw
*/
void ui_draw(UI_Element* element);

/**
* @brief check if the mouse is hovering over this ui element
* @param element the entity to check with
* @param mX the x position of the mouse point
* @param mY the y position of the mouse point
*/
Bool ui_clickable(UI_Element* element, float mX, float mY);

#endif