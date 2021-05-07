#include "simple_logger.h"

#include "starsabove_buildable.h"
#include "starsabove_planet.h"

typedef struct
{

	int num_buildables;
	Buildable* buildables;

} Buildable_Dict;

Buildable_Dict buildabledict = { 0 };

/* Buildable_Dict functions */
void buildabledict_load(SJson* buildabledict_json)
{
	int i;

	if (buildabledict_json == NULL)
	{
		slog("CANNOT LOAD NULL BUILDABLES JSON"); return;
	}

	if (sj_is_array(buildabledict_json) != 1)
	{
		slog("BUILDABLES JSON IS NOT ARRAY"); return;
	}

	buildabledict.num_buildables = sj_array_get_count(buildabledict_json);

	buildabledict.buildables = malloc(buildabledict.num_buildables * sizeof(Buildable));

	for (i = 0; i < buildabledict.num_buildables; i++)
	{
		buildabledict.buildables[i] = *buildable_fromJson(sj_array_get_nth(buildabledict_json, i));

		buildabledict.buildables[i].status = BLD_TEMPLATE;
	}

	slog("LOADED ALL BUILDABLE TEMPLATES");

	atexit(buildabledict_free);

}

void buildabledict_free()
{
	int i = 0;

	for (i = 0; i < buildabledict.num_buildables; i++)
	{
		buildable_free(&buildabledict.buildables[i]);

		&buildabledict.buildables[i] == NULL;
	}

	//memset(&buildabledict, 0, sizeof(Buildable_Dict));
}

Buildable* buildable_get_byname(char* key)
{
	int i = 0;

	for (i = 0; i < buildabledict.num_buildables; i++)
	{
		
		if (strcmp(buildabledict.buildables[i].name, key) == 0)
		{
			return &buildabledict.buildables[i];
		}

	}

	slog("No Building with the name %s", key);
	return NULL;

}

int num_ingame_buildables() { if (&buildabledict) { return buildabledict.num_buildables; } return 0;  }

/* Load and save buildables */
Buildable* buildable_fromJson(SJson* buildable_json)
{
	char name[128];
	int* status = -1;
	int* buildtime = 0;
	strcpy(name, sj_get_string_value(sj_object_get_value(buildable_json, "name")));

	if (sj_object_get_value(buildable_json, "status"))
	{
		sj_get_integer_value(sj_object_get_value(buildable_json, "status"), &status);
	}

	sj_get_integer_value(sj_object_get_value(buildable_json, "buildtime"), &buildtime);

	return buildable_new(
		status,
		name,
		resources_fromJson(sj_object_get_value(buildable_json, "input")),
		resources_fromJson(sj_object_get_value(buildable_json, "output")),
		resources_fromJson(sj_object_get_value(buildable_json, "resource unlocks")),
		resources_fromJson(sj_object_get_value(buildable_json, "supply")),
		resources_fromJson(sj_object_get_value(buildable_json, "supply capacity")),
		resources_fromJson(sj_object_get_value(buildable_json, "costs")),
		buildtime
	);

}

SJson* buildable_toJson(Buildable* buildable)
{
	int i = 0;
	SJson* buildable_json = sj_object_new();

	sj_object_insert(buildable_json, "name", sj_new_str(buildable->name));

	sj_object_insert(buildable_json, "input", resources_toJson(buildable->resource_input));
	sj_object_insert(buildable_json, "output", resources_toJson(buildable->resource_output));

	sj_object_insert(buildable_json, "supply", resources_toJson(buildable->supply));
	sj_object_insert(buildable_json, "supply capacity", resources_toJson(buildable->supply_cap));

	sj_object_insert(buildable_json, "resource unlocks", resources_toJson(buildable->resource_unlock));

	sj_object_insert(buildable_json, "status", sj_new_int(buildable->status));

	if (buildable->buildtime > 0) {

		sj_object_insert(buildable_json, "buildtime", sj_new_int(buildable->buildtime));

	}

	return buildable_json;
}

/* Create, copy and destroy buildables */
Buildable* buildable_new(int status, char* name, float* input, float* output, float* resource_unlocks, float* supply, float* supply_cap, float* costs, int buildtime)
{
	int i;

	Buildable* buildable = malloc(sizeof(Buildable));

	buildable->status = status;
	buildable->buildtime = buildtime;

	strcpy(buildable->name, name);

	buildable->resource_input = resourcelist_copy(input);
	buildable->resource_output = resourcelist_copy(output);
	buildable->supply = resourcelist_copy(supply);
	buildable->supply_cap = resourcelist_copy(supply_cap);
	buildable->resource_unlock = resourcelist_copy(resource_unlocks);
	buildable->costs = resourcelist_copy(costs);

	buildable->onNewTurn = buildable_onNewTurn;

	//buildable->_inuse = 0;

	return buildable;
}

Buildable* buildable_copy(Buildable* src)
{
	return buildable_new(src->status, src->name, src->resource_input, src->resource_output, src->resource_unlock, src->supply, src->supply_cap, src->costs, src->buildtime);
}

void buildable_free(Buildable* buildable)
{
	free(buildable->resource_input);
	free(buildable->resource_output);
}

/* Create a menustate for a building */
Menu_State* buildable_menustate_init(Buildable* buildable, Menu_State* previous_menustate)
{
	Menu_State* building_menustate = NULL;

	UI_Element* inputresources_textbox;
	UI_Element* outputresources_textbox;

	UI_Element* supply_textbox;

	UI_Element* resourceunlocks_textbox;

	char temp_text[128];

	if (!buildable)
	{
		slog("CANNOT MAKE MENUSTATE FOR NULL BUILDING"); return;
	}

	//Initialize Building Menu_State
	building_menustate = menu_state_new
	(
		previous_menustate,
		textbox_init(
			vector2d(10, 10),
			vector2d(200, 50),
			buildable->name,
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	//Add status textbox
	if (buildable->buildtime != 0)
	{
		sprintf(temp_text, "Status: %s | %i turns", buildable_status_names[buildable->status], buildable->buildtime);
	}
	
	else 
	{
		sprintf(temp_text, "Status: %s", buildable_status_names[buildable->status]);
	}

	menu_addTo
	(
		building_menustate->current_menu,
		textbox_init(
			vector2d(10, 10),
			vector2d(200, 50),
			temp_text,
			font_load("resources/fonts/futur.ttf", 12)
		)
	);

	//Input Resources
	//---------------

	///*
	if (!resourcelist_iszero(buildable->resource_input))
	{

		inputresources_textbox = textbox_init
		(
			vector2d(10, 10),
			vector2d(200, 50),
			"Input Resources",
			font_load("resources/fonts/futur.ttf", 12)
		);

		sprintf(temp_text, "%s Inputs", buildable->name);

		inputresources_textbox->signal = new_gameevent(
			NULL,
			NULL,
			"GETRESOURCES",
			NULL,
			0,
			resources_menustate_init(buildable->resource_input, building_menustate, temp_text, 1),
			0
		);

		menu_addTo
		(
			building_menustate->current_menu,
			inputresources_textbox
		);

	}
	//*/
	
	//Output Resources
	//----------------
	
	///*
	if (!resourcelist_iszero(buildable->resource_output))
	{
		outputresources_textbox = textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"Output Resources",
			font_load("resources/fonts/futur.ttf", 12)
		);

		sprintf(temp_text, "%s Outputs", buildable->name);

		outputresources_textbox->signal = new_gameevent(
			NULL,
			NULL,
			"GETRESOURCES",
			NULL,
			0,
			resources_menustate_init(buildable->resource_output, building_menustate, temp_text, 1),
			0
		);

		menu_addTo
		(
			building_menustate->current_menu,
			outputresources_textbox
		);

	}
	//*/

	//Supply out of capacity
	//----------------

	///*
	if (!resourcelist_iszero(buildable->supply_cap))
	{

		supply_textbox = textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"Supply",
			font_load("resources/fonts/futur.ttf", 12)
		);

		supply_textbox->signal = new_gameevent(
			NULL,
			NULL,
			"GETRESOURCES",
			NULL,
			0,
			resources_comparative_menustate_init(buildable->supply, buildable->supply_cap, building_menustate, temp_text),
			0
		);

		menu_addTo
		(
			building_menustate->current_menu,
			supply_textbox
		);
	}
	//*/

	//Resource Unlocks
	//----------------
	
	///*
	if (!resourcelist_iszero(buildable->resource_unlock))
	{

		resourceunlocks_textbox = textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			"Resource Unlocks",
			font_load("resources/fonts/futur.ttf", 12)
		);

		sprintf(temp_text, "%s Outputs", buildable->name);

		resourceunlocks_textbox->signal = new_gameevent(
			NULL,
			NULL,
			"GETRESOURCES",
			NULL,
			0,
			resources_menustate_init(buildable->resource_unlock, building_menustate, temp_text, 1),
			0
		);

		menu_addTo
		(
			building_menustate->current_menu,
			resourceunlocks_textbox
		);
	}

	//*/

	menu_state_hide(building_menustate);

	return building_menustate;

}

Menu_State* buildable_construction_menustate_all(Menu_State* previous_menustate, Planet* planet, char* system_name)
{
	int i;
	int j;
	Bool hasbuildable;

	Menu_State* construction_menu;
	Menu_State* this_buildable_menu;

	Menu_State* temp_menustate;

	UI_Element* buildable_textbox;
	UI_Element* info_textbox;
	UI_Element* construct_textbox;

	UI_Element* temp_uielem;

	Buildable* this_buildable;

	construction_menu = menu_state_new(
		previous_menustate,
		textbox_init(
			vector2d(10, 10),
			vector2d(200, 50),
			"Construct a Building",
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	for (i = 0; i < num_ingame_buildables(); i++)
	{
		this_buildable = &buildabledict.buildables[i];

		//Check if the planet has this buildable
		hasbuildable = 0;

		for (j = 0; j < planet->num_buildings; j++)
		{
			if (strcmp(planet->buildings[j].name, this_buildable->name) == 0)
			{
				hasbuildable = 1;
				break;
			}
		}

		if (hasbuildable == 0)
		{
			/*Create the buildable textbox and menustate*/
			buildable_textbox = textbox_init
			(
				vector2d(10, 10),
				vector2d(200, 50),
				this_buildable->name,
				font_load("resources/fonts/futur.ttf", 12)
			);

			this_buildable_menu = menu_state_new
			(
				construction_menu,
				textbox_init
				(
					vector2d(10, 10),
					vector2d(200, 50),
					this_buildable->name,
					font_load("resources/fonts/futur.ttf", 16)
				),
				NULL,
				vector2d(10, 10),
				0,
				5
			);


			/*Create the buildable's construct button*/

			construct_textbox = textbox_init
			(
				vector2d(10, 10),
				vector2d(200, 50),
				"Construct Building",
				font_load("resources/fonts/futur.ttf", 12)
			);

			construct_textbox->signal = new_gameevent
			(
				system_name,
				planet->name,
				"CONSTRUCT_BUILDING",
				this_buildable->name,
				0,
				NULL,
				0
			);

			menu_addTo(this_buildable_menu->current_menu, construct_textbox);


			/*Create the buildable's info textbox*/
			
			info_textbox = textbox_init
			(
				vector2d(10, 10),
				vector2d(200, 50),
				"Building Information",
				font_load("resources/fonts/futur.ttf", 12)
			);

			info_textbox->signal = new_gameevent
			(
				system_name,
				planet->name,
				"BUILDING_INFO",
				this_buildable->name,
				0,
				buildable_menustate_init(this_buildable, this_buildable_menu),
				0
			);

			menu_addTo(this_buildable_menu->current_menu, info_textbox);

			//Add the "cost" info to the buildable menustate.
			temp_menustate = this_buildable_menu;

			temp_uielem = textbox_init
			(
				vector2d(10, 10),
				vector2d(200, 50),
				"Resource Costs",
				font_load("resources/fonts/futur.ttf", 12)
			);

			temp_uielem->signal = new_gameevent
			(
				system_name,
				planet->name,
				"BUILDING_COSTS",
				this_buildable->name,
				0,
				resources_menustate_init(this_buildable->costs, this_buildable_menu, "Resource Costs", 1),
				0
			);

			menu_addTo
			(
				temp_menustate->current_menu,
				temp_uielem
			);

			menu_state_hide(temp_uielem->signal->menu_state);


			/*Wrapping things up, connect to the buildable main menu*/

			menu_state_hide(this_buildable_menu);

			buildable_textbox->signal = new_gameevent
			(
				system_name,
				planet->name,
				"BUILDINGCONSTRUCTION_INFO",
				this_buildable->name,
				0,
				this_buildable_menu,
				0
			);

			menu_addTo(construction_menu->current_menu, buildable_textbox);
		}

	}

	menu_state_hide(construction_menu);

	return construction_menu;
}

float* buildable_onNewTurn(Buildable* self)
{
	self->buildtime = max(0, self->buildtime - 1);

	if ((self->buildtime == 0) && (self->status == (int)BLD_CONSTRUCTING))
	{
		self->status = (int)BLD_ACTIVE;
	}

	return resourcelist_subtract(self->resource_output, self->resource_input);
}