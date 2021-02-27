#pragma once
#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_system.h"

Entity* system_spawn(Vector2D position)
{

    Entity* ent = NULL;

    //Create the new entity
    ent = entity_new();

    if (!ent)
    {
        slog("Failed to spawn a system");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/gameobjects/sun.png", 128, 128, 1);

    vector2d_copy(ent->position, position);

    ent->frameRate = 0;
    ent->frameCount = 1;
    
    /*ent->collider_box = box_new();

    vector2d_copy(boxsize, vector2d(ent->sprite->frame_w, ent->sprite->frame_h));

    vector2d_copy(ent->collider_box->position, ent->position);

    vector2d_add(ent->collider_box->extremity, position, boxsize);*/

    //Create collider circle
    ent->collider_circle = circle_new();

    ent->collider_circle->radius = 32;

    vector2d_copy(ent->collider_circle->position, ent->position);
    

    //Center circle collider to center of sprite
    ent->collider_circle->position.x += ent->sprite->frame_w/2;
    ent->collider_circle->position.y += ent->sprite->frame_h/2;
    

    slog("System created!");
    return ent;
}

/*Bottom*/
