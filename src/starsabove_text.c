#include "simple_logger.h"
#include "gf2d_graphics.h"

#include "starsabove_text.h"
#include "gfc_text.h"
#include "gfc_color.h"

#include "gf2d_graphics.h"

typedef struct
{
	Font* fontList;
	Uint32 maxFonts;
} FontManager;

static FontManager font_manager = { 0 };

void font_delete(Font* font)
{
	if (!font) return;
	if (font->font != NULL) TTF_CloseFont(font->font);
	memset(font, 0, sizeof(Font));
}

void font_manager_close()
{
	int i;

	if (font_manager.fontList != NULL)
	{
		for (i = 0; i < font_manager.maxFonts; i++)
		{
			font_delete(&font_manager.fontList[i]);
		}
	}

	free(font_manager.fontList);
	memset(&font_manager, 0, sizeof(FontManager));
}

void font_init(Uint32 maxFonts)
{
	if (!maxFonts)
	{
		slog("Cannot allocate for 0 fonts!"); return;
	}

	if ((!TTF_WasInit()) && (TTF_Init() == -1)) { //If cannot init SDL_ttf
		slog("TTF_Init: %s\n", TTF_GetError());
		return;
	}

	font_manager_close(); //Close if one already exists

	font_manager.maxFonts = maxFonts;
	font_manager.fontList = (Font*)gfc_allocate_array(sizeof(Font), maxFonts);

	atexit(font_manager_close);
}

Font* font_new()
{
	int i;
	for (i = 0; i < font_manager.maxFonts; i++)
	{
		if (font_manager.fontList[i]._refCount == 0)
		{
			font_manager.fontList[i]._refCount = 1;
			return &font_manager.fontList[i];
		}
	}
}

Font* font_get_by_file_point(const char* filename, int ptsize)
{
	int i = 0;

	for (i = 0; i < font_manager.maxFonts; i++)
	{
		if (font_manager.fontList[i]._refCount == 0) continue;

		if (gfc_line_cmp(font_manager.fontList[i].file, filename) != 0) continue;

		if (font_manager.fontList[i].ptsize != ptsize) continue;

		return &font_manager.fontList[i];
	}

	return NULL;
}

Font* font_load(const char* filename, int ptsize)
{
	Font* font;

	font = font_get_by_file_point(filename, ptsize);

	if (font != NULL) //Already in memory, increase refcount and return
	{
		font->_refCount++;
		return font;
	}

	font = font_new();

	if (!font)
	{
		slog("Failed to get a new font slot!"); return NULL;
	}

	font->ptsize = ptsize;
	gfc_line_cpy(font->file, filename);

	font->font = TTF_OpenFont(filename, ptsize);

	if (font->font == NULL)
	{
		slog("Failed to open font \"%s\"", filename); return NULL;
	}

	return font;
}

void font_free(Font* font)
{
	if (!font) return;
	font->_refCount--;

	if (font->_refCount == 0) font_delete(font);
}

void font_render(Font* font, char* text, Vector2D pos, Color color)
{
	SDL_Surface* surface;
	SDL_Texture* texture;

	SDL_Rect rect;

	if (!font)
	{
		slog("No font provided to render!");
		return;
	}

	if (!text)
	{
		slog("No text provided to render!");
	}

	surface = TTF_RenderText_Blended(font->font, (const char*)text, gfc_color_to_sdl(color));

	if (!surface)
	{
		slog("Failed to render to surface");
		return;
	}

	surface = gf2d_graphics_screen_convert(&surface);

	if (!surface)
	{
		slog("Failed to convert rendered text to graphics format \"%s\"", font->file);
		return;
	}

	texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);

	if (!texture)
	{
		slog("Failed to convert surface text to texture \"%s\"", text);
		SDL_FreeSurface(surface);
		return;
	}
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	rect = gfc_sdl_rect((Sint32)pos.x, (Sint32)pos.y, surface->w, surface->h);

	SDL_UpdateTexture(texture,
		NULL,
		surface->pixels,
		surface->pitch);

	SDL_RenderCopyEx(gf2d_graphics_get_renderer(),
		texture,
		NULL,
		&rect,
		0,
		NULL,
		0
	);

	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}
