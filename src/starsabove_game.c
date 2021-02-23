#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "simple_logger.h"

#include "starsabove_game.h"

void prepare_game()
{
    /* Starting the entity manager */
    entity_manager_init(100);

    system_spawn(vector2d(100, 100));

    system_spawn(vector2d(300, 100));

}

void starsabove_loop()
{
	entity_manager_update_entities();

	entity_manager_draw_entities();
}