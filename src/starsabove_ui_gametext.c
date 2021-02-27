#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_ui_gametext.h"

UI_Element* gametext_init(Vector2D position)
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

    element->sprite = gf2d_sprite_load_all("images/ui/hud_basic.png", 1500, 200, 1);

    vector2d_copy(element->position, position);

    element->frameRate = 0;
    element->frameCount = 1;

    element->collider_box = box_new();

    vector2d_copy(boxsize, vector2d(element->sprite->frame_w, element->sprite->frame_h));

    vector2d_copy(element->collider_box->position, element->position);

    vector2d_add(element->collider_box->extremity, position, boxsize);

    slog("System created!");
    return element;
}

/*Bottom*/
