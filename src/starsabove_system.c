#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

Entity* system_spawn(Vector2D position)
{

    Entity* ent = NULL; //slog("SHOULD NOT HAVE TO INITIALIZE ENT TO NULL, ASK FOR HELP!");

    //Create the new entity
    ent = entity_new();

    if (!ent)
    {
        slog("Failed to spawn a system");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/gameobjects/sun.png", 256, 256, 1);

    vector2d_copy(ent->position, position);

    ent->frameRate = 0;
    ent->frameCount = 1;
    
    circle_new(ent->collider_circle, 128, ent->position);

    slog("System created!");
    return ent;
}

/*Bottom*/
