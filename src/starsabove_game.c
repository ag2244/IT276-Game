#include <SDL.h>

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

#include "simple_logger.h"

#include "starsabove_game.h"

GameState gameState = { 0 };

void prepare_game()
{
    /* Starting the entity manager */
    entity_manager_init(100);

	/* Starting the ui manager */
	ui_manager_init(50);

	gametext_init(vector2d(0, 700));

    system_spawn("System1", vector2d(500, 300));

    system_spawn("System2", vector2d(300, 400));

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
	
	//Get the UI Manager
	ui_manager = ui_manager_get();

	if (!ui_manager) {
		return NULL;
	}

	//Go through each element in the UI
	for (i = 0; i < ui_manager->max_elements; i++)
	{
		//If the element at index i is not in use, continue
		if (ui_manager->element_list[i]._inuse == 0) continue;

		//If the element is a clickable and the mouse is hovering over it, set the gameState.currentClickable_ui to point to it and return True.
		if (ui_clickable(&ui_manager->element_list[i], mX, mY)) 
		{
			gameState.currentClickable_ui = &ui_manager->element_list[i];
			gameState.currentClickable_entity = NULL;
			return 1;
		}
			
	}

	//Now to do the same but for the entity manager
	entity_manager = entity_manager_get();

	if (!entity_manager) {
		return NULL;
	}

	//Go through each entity in the entity manager
	for (i = 0; i < entity_manager->max_entities; i++)
	{
		//If the entity at index i is not in use, continue
		if (entity_manager->entity_list[i]._inuse == 0) continue;

		//If the entity is a clickable and the mouse is hovering over it, set the gameState.currentClickable_entity to point to it and return True.
		if (entity_clickable(&entity_manager->entity_list[i], mX, mY))
		{
			gameState.currentClickable_entity = &entity_manager->entity_list[i];
			gameState.currentClickable_ui = NULL;
			return 1;
		}
	}

	//No clickable is being hovered over
	gameState.currentClickable_ui = NULL;
	gameState.currentClickable_entity = NULL;
	return 0;

}

void onClick_left() 
{
	if (gameState.currentClickable_entity) {
		entity_onClick(gameState.currentClickable_entity);
	};
}

void processKeys(Uint8 keys, Uint32 mouse) {

	SDL_Event frame_event;

	while (SDL_PollEvent(&frame_event)) {

		switch (frame_event.type) {

		case SDL_MOUSEBUTTONDOWN:

			if (frame_event.button.button == SDL_BUTTON_LEFT)
				onClick_left();

			break;

		default:
			break;
		}
	}

}