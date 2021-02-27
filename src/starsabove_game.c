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

	/* Starting the ui manager */
	ui_manager_init(50);

	gametext_init(vector2d(0, 700));

    system_spawn(vector2d(500, 300));

    system_spawn(vector2d(300, 400));

}

void starsabove_loop()
{
	entity_manager_update_entities();

	entity_manager_draw_entities();

	ui_manager_update();

	ui_manager_draw();
}

Bool starsabove_hoverDetection(float mX, float mY) {

    int i;
	UI_Manager* ui_manager;
	EntityManager* entity_manager;
	
	ui_manager = ui_manager_get();

	for (i = 0; i < ui_manager->max_elements; i++)
	{
		if (ui_manager->element_list[i]._inuse == 0) continue;

		if (ui_clickable(&ui_manager->element_list[i], mX, mY))
			return 1;
	}

	entity_manager = entity_manager_get();

	if (!entity_manager) {
		return NULL;
	}

	for (i = 0; i < entity_manager->max_entities; i++)
	{
		if (entity_manager->entity_list[i]._inuse == 0) continue;

		if (entity_clickable(&entity_manager->entity_list[i], mX, mY))
			return 1;
	}

	return 0;

}