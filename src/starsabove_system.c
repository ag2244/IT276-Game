#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

System_Data* system_data_new(char* name)
{
    return malloc(sizeof(System_Data));
}

void system_onClick(struct Entity_s* self) {

    System_Data* system_data = self->data;

    if (system_data->name != NULL)
        slog("Clicked on STAR SYSTEM %s", system_data->name);
}

Entity* system_spawn(char* name, Vector2D position)
{

    Entity* ent = NULL;
    System_Data* thisSystem = system_data_new(name);

    //Create the new entity
    ent = entity_new();

    if (!ent)
    {
        slog("Failed to spawn a system");
        return NULL;
    }

    //Load sprite and related info
    ent->sprite = gf2d_sprite_load_all("images/gameobjects/star.png", 64, 64, 1);

    vector2d_copy(ent->position, position);

    ent->frameRate = 0;
    ent->frameCount = 1;

    //Create collider circle
    ent->collider_circle = circle_new();

    ent->collider_circle->radius = 32;

    vector2d_copy(ent->collider_circle->position, ent->position);
    

    //Center circle collider to center of sprite
    ent->collider_circle->position.x += ent->sprite->frame_w/2;
    ent->collider_circle->position.y += ent->sprite->frame_h/2;

    //Load system details
    thisSystem->name = name;

    (struct System_Data*)ent->data = thisSystem;

    ent->onClick = system_onClick;

    slog("System created!");
    return ent;
}

/*Bottom*/
