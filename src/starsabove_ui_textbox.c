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

    vector2d_copy(
        element->text_position_relative, 
            vector2d(
                15,
                (element->spriteBorder->frame_h - font->ptsize) / 2
            )
        );

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

    element->onClick = textbox_onClick;

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
    newMenu->beginning->next = NULL;

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

Menu_State* menu_state_addTo(Menu_State* old, Menu* newMenu)
{

}

Menu_State* menu_state_back(Menu_State* menu)
{

}

void ui_node_addTo(Menu* menu, UI_Node* node, UI_Element* element)
{
    if (node->next != NULL)
    {
        ui_node_addTo(menu, node->next, element);
        return;
    }

    element->position = vector2d(
        node->element->position.x + menu->spacing_x,
        node->element->position.y + menu->spacing_y + node->element->size.y
    );

    element->collider_box->position = vector2d(
        node->element->collider_box->position.x + menu->spacing_x,
        node->element->collider_box->position.y + menu->spacing_y + node->element->collider_box->size.y
    );

    node->next = malloc(sizeof(UI_Node));
    node->next->element = element;
    node->next->next = NULL;
}

void menu_addTo(Menu* menu, UI_Element* element)
{
    ui_node_addTo(menu, menu->beginning, element);
}

void ui_node_free(UI_Node node)
{
    if (node.next != NULL) 
        ui_node_free(*node.next);

    ui_free(node.element);

    free(&node);
}

void menu_free(Menu* menu)
{
    ui_free(&menu->title);
    ui_free(&menu->next_button);
    ui_free(&menu->previous_button);

    ui_node_free(*menu->beginning);

    free(menu);
}

void menu_state_free(Menu_State* menu_state)
{
    menu_free(menu_state->current_menu);

    menu_state_free(menu_state->previous_menu_state);

    free(menu_state);
}

void textbox_onClick(UI_Element* element, Game_Event* event_reciever)
{
    if (element == NULL)
    {
        slog("Cannot click on null element!");
        return;
    }

    if (element->signal)
    {
        gfc_line_cpy(event_reciever->command, element->signal->command);
        gfc_line_cpy(event_reciever->target_id, element->signal->target_id);
        gfc_line_cpy(event_reciever->descriptor, element->signal->descriptor);
        event_reciever->qty = element->signal->qty;
        event_reciever->_sent = 1;
    }
}