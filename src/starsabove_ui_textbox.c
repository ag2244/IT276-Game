#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_ui_textbox.h"

UI_Element* textbox_init(Vector2D position, Vector2D size, char* text, Font* font)
{

    UI_Element* element = NULL;
    Vector2D boxsize;

    //Create the new elementity
    element = ui_new();

    if (!element)
    {
        slog("Failed to spawn a system");
        return NULL;
    }

    //Load sprites
    element->spriteBorder = gf2d_sprite_load_all("images/ui/textbox_outer.png", size.x, size.y, 1);
    element->spriteMain = gf2d_sprite_load_all("images/ui/textbox_inner.png", max(1, size.x - 10), max(1, size.y - 10), 1);

    element->offset = vector2d(5, 5);

    //Load text rendering info
    element->text = text;
    element->font = font;
    element->text_position_relative = vector2d(10, (element->spriteBorder->frame_h - font->ptsize) / 2);
    element->text_color = gfc_color(29 / 255, 50 / 255, 54 / 255, 0);

    //Load position
    vector2d_copy(element->position, position);

    element->frameRate = 0;
    element->frameCount = 1;

    //Get a collider
    element->collider_box = box_new();

    vector2d_copy(boxsize, vector2d(element->spriteBorder->frame_w, element->spriteBorder->frame_h));

    vector2d_copy(element->collider_box->position, element->position);

    vector2d_add(element->collider_box->extremity, position, boxsize);

    slog("System created!");
    return element;
}

/*Bottom*/
