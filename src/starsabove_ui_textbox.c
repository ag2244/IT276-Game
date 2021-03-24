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
        slog("Failed to create a textbox");
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

    element->text_color = gfc_color(0, 0, 0, 0);

    //Load position, size
    vector2d_copy(element->position, position);
    vector2d_copy(element->size, size);

    element->frameRate = 0;
    element->frameCount = 1;

    //Get a collider
    element->collider_box = box_new();

    //vector2d_copy(boxsize, vector2d(element->spriteBorder->frame_w, element->spriteBorder->frame_h));

    vector2d_copy(element->collider_box->position, element->position);

    vector2d_copy(element->collider_box->size, vector2d(element->spriteBorder->frame_w, element->spriteBorder->frame_h));

    element->onClick = textbox_onClick;

    //slog("Textbox created with text \"%s\"!", text);
    return element;
}

Menu* menu_init(UI_Element* title, UI_Element* beginning, Vector2D position, int spacing_x, int spacing_y)
{
    Menu* newMenu = malloc(sizeof(Menu));

    newMenu->title = title;

    newMenu->beginning = malloc(sizeof(UI_Node));

    //Set up the beginning textbox

    newMenu->beginning->element = beginning;
    if (newMenu->beginning->element != NULL)
    {
        newMenu->beginning->element->position = vector2d(position.x + spacing_x, position.y + title->size.y + spacing_y);
        vector2d_copy(newMenu->beginning->element->collider_box->position, newMenu->beginning->element->position);
    }

    newMenu->beginning->next = NULL;
    newMenu->position = position;

    newMenu->spacing_x = spacing_x;
    newMenu->spacing_y = spacing_y;

    newMenu->next_button = NULL;
    newMenu->previous_button = NULL;

    return newMenu;
}

Menu_State* menu_state_new(Menu_State* previous_menu_state, UI_Element* title, UI_Element* beginning, Vector2D position, int spacing_x, int spacing_y)
{
    Menu_State* new_state = malloc(sizeof(new_state));

    Menu* newMenu = menu_init(title, beginning, position, spacing_x, spacing_y);

    new_state->current_menu = newMenu;

    new_state->previous_menu_state = previous_menu_state;

    if (new_state->previous_menu_state != NULL)
        menu_state_hide(new_state->previous_menu_state);

    return new_state;
}

Menu_State* menu_state_getsafe(Menu_State* menustate)
{
    Menu_State* menustate_ptr = menustate;
    return menustate_ptr;
}

Menu_State* menu_state_addTo(Menu_State* old, Menu* newMenu)
{
    Menu_State* new_state = malloc(sizeof(Menu_State));

    new_state->current_menu = newMenu;
    new_state->previous_menu_state = old;

    menu_state_hide(new_state->previous_menu_state);
    menu_state_show(new_state);

    return new_state;
}

Menu_State* menu_state_back(Menu_State* menu)
{
    Menu_State* prev = menu->previous_menu_state;

    menu->previous_menu_state = NULL;
    menu_state_hide(menu);

    if (prev == NULL)
    {
        menu_state_free(menu);
    }

    return prev;
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
    if (menu->beginning->element == NULL)
    {
        element->position = vector2d(menu->position.x + menu->spacing_x, menu->position.y + menu->title->size.y + menu->spacing_y);
        vector2d_copy(element->collider_box->position, element->position);

        menu->beginning->element = element;
    }
    
    else
        ui_node_addTo(menu, menu->beginning, element);
}

//Get to the root of a Menu_State super-structure
Menu_State* menu_state_root(Menu_State* menu_state)
{
    if (menu_state->previous_menu_state != NULL)
    {
        return menu_state_root(menu_state->previous_menu_state);
    }

    return menu_state;
}

//Hide a menu state, its menu, and its elements

void ui_node_hide(UI_Node* node)
{
    if (node == NULL)
    {
        slog("Cannot hide NULL node"); return;
    }

    node->element->hidden = 1;
    
    if (node->next != NULL)
    {
        ui_node_hide(node->next);
    }
}

void menu_hide(Menu* menu)
{
    if (menu == NULL)
    {
        slog("Cannot hide NULL menu"); return;
    }

    menu->title->hidden = 1;

    if (menu->beginning->element != NULL)
    {
        ui_node_hide(menu->beginning);
    }
}

void menu_state_hide(Menu_State* menu_state)
{

    if (menu_state == NULL)
    {
        slog("Cannot hide NULL menu_state"); return;
    }

    if (menu_state->current_menu != NULL)
    {
        menu_hide(menu_state->current_menu);
    }
        

    if (menu_state->previous_menu_state != NULL)
    {
        menu_state_hide(menu_state->previous_menu_state);
    }

}

//Show a menu state, its menu, and its elements

void ui_node_show(UI_Node* node)
{

    if (node == NULL)
    {
        slog("Cannot show NULL node"); return;
    }

    node->element->hidden = 0;

    if (node->next != NULL)
    {
        ui_node_show(node->next);
    }
}

void menu_show(Menu* menu)
{
    if (menu == NULL)
    {
        slog("Cannot show NULL menu"); return;
    }

    menu->title->hidden = 0;

    if (menu->beginning->element != NULL)
        ui_node_show(menu->beginning);
}

void menu_state_show(Menu_State* menu_state)
{
    if (menu_state == NULL)
    {
        slog("Cannot show NULL menu_state"); return;
    }

    menu_show(menu_state->current_menu);

    if (menu_state->previous_menu_state != NULL)
        menu_state_hide(menu_state->previous_menu_state);

}

//Free structs

void ui_node_free(UI_Node* node)
{

    if (node->next != NULL)
    {
        ui_node_free(node->next);
    }

    ui_free(node->element);
}

void menu_free(Menu* menu)
{

    if (!menu)
    {
        slog("CANNOT FREE NULL MENU"); return;
    }

    if(menu->previous_button != NULL)
    {
        ui_free(menu->previous_button);
    }

    if (menu->next_button != NULL)
    {
        ui_free(menu->next_button);
    }

    if (menu->title != NULL)
    {
        ui_free(menu->title);
    }

    if (menu->beginning != NULL)
    {
        ui_node_free(menu->beginning);
    }

    free(menu);

}

void menu_state_free(Menu_State* menu_state)
{

    if (!menu_state)
    {
        slog("CANNOT FREE NULL MENU_STATE"); return;
    }

    menu_free(menu_state->current_menu);

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
        gameevent_copy(event_reciever, element->signal);
    }
}