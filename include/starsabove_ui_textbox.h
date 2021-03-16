#pragma once

#ifndef __UI_TEXTBOX_H__
#define __UI_TEXTBOX_H__

#include "starsabove_entity.h"
#include "starsabove_ui_base.h"

/**
* @brief Initialize a textbox
* @param position The screen position to draw the textBox
* @param size The size of the textBox
* @return NULL on error, or a pointer to a new player entity
*/
UI_Element * textbox_init(Vector2D position, Vector2D size, char* text, Font* font);

#endif
