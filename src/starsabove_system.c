#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

//Use for pointing to the data for an individual system
System_Data* system_data;

System_Data* system_data_new(char* name)
{
    return malloc(sizeof(System_Data));
}

char* system_name(Entity* self)
{
    system_data = self->data;
    //if (system_data->name != "System1") slog("\"%s\"", system_data->name); 
    return system_data->name;
}

void system_onClick(Entity* self) {

    system_data = self->data;

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

    ent->name = system_name;

    slog("System \"%s\" created!", ent->name(ent));
    return ent;
}

/*Bottom*/
