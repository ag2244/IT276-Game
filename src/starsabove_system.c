#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

//Use for pointing to the data for an individual system
System_Data* system_data;

System_Data* system_data_new()
{
    return malloc(sizeof(System_Data));
}

void system_onClick(Entity* self) {
    if (self == NULL)
    {
        slog("Cannot click on NULL entity!");
        return NULL;
    }

    /*if (self->name != NULL)
        slog("Clicked on STAR SYSTEM \"%s\"", self->name);*/
}

SJson* system_toJson(Entity* self)
{
    SJson* systemJson;
    SJson* array_position;

    systemJson = sj_object_new();
    array_position = sj_array_new();

    if (self == NULL)
    {
        slog("Cannot convert NULL system to json!");
        return NULL;
    }

    //Insert the system name
    sj_object_insert(systemJson, "name", sj_new_str(self->name));

    //Get the position array and insert it into the system json
    sj_array_append(array_position, sj_new_float(self->position.x));
    sj_array_append(array_position, sj_new_float(self->position.y));

    sj_object_insert(systemJson, "position", array_position);

    return systemJson;

    return sj_object_new();
}

Entity* system_spawn(char* name, Vector2D position)
{

    Entity* ent = NULL;
    System_Data* thisSystem = system_data_new(name);

    //Create the new entity
    ent = entity_new_name(name);

    if (!ent)
    {
        slog("Failed to spawn a system");
        //return NULL;
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
    //ent->name = name;

    (struct System_Data*)ent->data = thisSystem;

    //Functions

    ent->onClick = system_onClick;
    ent->toJson = system_toJson;

    //Done!
    slog("System \"%s\" created!", ent->name);
    return ent;
}

/*Bottom*/
