#include "simple_logger.h"
#include "gfc_vector.h"

#include "starsabove_player.h"

Entity* player_spawn(Vector2D position)
{

	Entity* ent = NULL; slog("SHOULD NOT HAVE TO INITIALIZE ENT TO NULL, ASK FOR HELP!");

    //Create the new entity
    ent = entity_new();

    if (!ent)
    {
        slog("Failed to create player entity");
        return NULL;
    }

    ent->sprite = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);

    vector2d_copy(ent->position, position);

    ent->frameRate = 0.1;
    ent->frameCount = 16;

    vector2d_copy(ent->velocity, vector2d(1, 1));

    slog("Player created!");
    return ent;
}

/*Bottom*/