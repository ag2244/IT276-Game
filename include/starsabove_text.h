#ifndef __TEXT_H__
#define __TEXT_H__

#include <stdlib.h>
#include <SDL_ttf.h>

#include "gfc_text.h"
#include "gfc_types.h"
#include "gfc_color.h"
#include "gfc_vector.h"

typedef struct
{
	Uint32 _refCount; //How many times this resource has been used
	TTF_Font* font;
	TextLine file; // Filename of the font we loaded
	int ptsize; // The font size loaded

} Font;

/**
* @brief Initialize font resource manager
* @param maxFonts how many fonts to support
*/
void font_init(Uint32 maxFonts);

/**
* @brief Load a font from a file
* @param filename the name of the fontfile
* @param ptsize the font size
*/
Font* font_load(const char* filename, int ptsize);

/**
* @brief Free a font
* @param font the font to free
*/
void font_free(Font* font);

/**
* @brief Render text to the screen
* @param font the font to render the text with
* @param text the text to render
* @param pos the position to render the text
* @param color the text color
*/
void font_render(Font* font, char* text, Vector2D pos, Color color);
#endif