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
    gfc_line_cpy(element->text, text);
    element->font = font;
    element->text_position_relative = 
        vector2d(
            10, 
            (element->spriteBorder->frame_h - font->ptsize) / 2);

    element->text_color = gfc_color(29 / 255, 50 / 255, 54 / 255, 0);

    //Load position, size
    vector2d_copy(element->position, position);
    vector2d_copy(element->size, size);

    element->frameRate = 0;
    element->frameCount = 1;

    //Get a collider
    element->collider_box = box_new();

    vector2d_copy(boxsize, vector2d(element->spriteBorder->frame_w, element->spriteBorder->frame_h));

    vector2d_copy(element->collider_box->position, element->position);

    vector2d_copy(element->collider_box->size, boxsize);

    slog("Textbox created with text \"%s\"!", text);
    return element;
}

Menu* menu_init(UI_Element* title, UI_Element* beginning, Vector2D position, int spacing_x, int spacing_y)
{
    Menu* newMenu = malloc(sizeof(Menu));

    newMenu->title = *title;

    //Set up the beginning textbox
    beginning->position = vector2d(position.x + spacing_x, position.y + title->size.y + spacing_y);
    vector2d_copy(beginning->collider_box->position, beginning->position);

    newMenu->beginning = malloc(sizeof(UI_Node));
    newMenu->beginning->element = beginning;

    newMenu->position = position;

    newMenu->spacing_x = spacing_x;
    newMenu->spacing_y = spacing_y;

    return newMenu;
}

Menu_State* menu_state_new(Menu_State* previous_menu_state, UI_Element* title, UI_Element* beginning, Vector2D position, int spacing_x, int spacing_y)
{
    Menu_State* new_state = malloc(sizeof(new_state));

    Menu* newMenu = menu_init(title, beginning, position, spacing_x, spacing_y);

    new_state->current_menu = newMenu;
    new_state->previous_menu_state = &previous_menu_state;

    return new_state;
}

/*Bottom*/
