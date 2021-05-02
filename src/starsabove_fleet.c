#include "simple_logger.h"

#include "starsabove_fleet.h"

#include "starsabove_nation.h"
#include "starsabove_system.h"
#include "starsabove_game_resources.h"

typedef struct
{
	int numships;
	Ship* ship_templates;
} 
Ship_Dict;

Ship_Dict shipdict = { 0 };


Fleet* fleet_fromlist(Fleet* fleet, int i)
{
	if (fleet[i]._inuse == 1)
	{
		return &fleet[i];
	}

	return NULL;
}

void shiptemplates_load(SJson* ships_json)
{

	int i;

	if (ships_json == NULL)
	{
		slog("CANNOT LOAD NULL SHIPS JSON"); return;
	}

	if (sj_is_array(ships_json) != 1)
	{
		slog("SHIP JSON IS NOT ARRAY"); return;
	}

	shipdict.numships = sj_array_get_count(ships_json);

	shipdict.ship_templates = malloc(shipdict.numships * sizeof(Ship));

	for (i = 0; i < shipdict.numships; i++)
	{
		shipdict.ship_templates[i] = *ship_fromJson(sj_array_get_nth(ships_json, i), NULL);

		shipdict.ship_templates[i].status = (int)SHIP_TEMPLATE;

		shipdict.ship_templates[i].health = 100;
	}

	slog("LOADED ALL SHIP TEMPLATES");

	atexit(shiptemplates_free);
}

void shiptemplates_free()
{
	int i;

	for (i = 0; i < shipdict.numships; i++)
	{
		ship_free(&shipdict.ship_templates[i]);
	}

	shipdict.numships = 0;
}

Ship* getshipbyname(char shiptype[128])
{
	int i;

	for (i = 0; i < shipdict.numships; i++)
	{
		if (strcmp(shiptype, shipdict.ship_templates[i].type) == 0)
		{
			return &shipdict.ship_templates[i];
		}
	}

	return NULL;
}

Ship* ship_copy(Ship* src, Fleet* fleet)
{
	return ship_init(src->type, src->maintenance, src->costs, src->health, src->status, fleet);
}

Ship* ship_fromJson(SJson* ship_json, Fleet* fleet)
{

	int* health = 0;
	int* status = 0;

	if (!ship_json)
	{
		slog("CANNOT LOAD NULL JSON SHIP"); return NULL;
	}

	sj_get_integer_value(sj_object_get_value(ship_json, "health"), &health);

	sj_get_integer_value(sj_object_get_value(ship_json, "status"), &status);

	return ship_init(
		sj_get_string_value(sj_object_get_value(ship_json, "type")),
		resourcelist_copy(resources_fromJson(sj_object_get_value(ship_json, "maintenance"))),
		resourcelist_copy(resources_fromJson(sj_object_get_value(ship_json, "costs"))),
		health,
		status,
		fleet);
}

Ship* ship_init(char shiptype[128], float* maintenance, float* costs, int health, int status, Fleet* fleet)
{
	Ship* ship = malloc(sizeof(Ship));

	strcpy(ship->type, shiptype);
	ship->fleet = fleet;

	ship->maintenance = resourcelist_copy(maintenance);
	ship->costs = resourcelist_copy(costs);

	ship->health = health;
	ship->status = status;

	ship->_inuse = 1;

	return ship;
}

Fleet* fleet_fromjson(SJson* fleet_json)
{
	int i; int j;

	SJson* ships_json;

	Fleet* fleet;
	int* status;

	if (!fleet_json)
	{
		slog("CANNOT LOAD FLEET FROM NULL JSON"); return NULL;
	}

	sj_get_integer_value(sj_object_get_value(fleet_json, "status"), &status);

	ships_json = sj_object_get_value(fleet_json, "ships");

	if (!ships_json)
	{
		slog("CANNOT LOAD FLEET SHIPS FROM NULL JSON"); return NULL;
	}

	fleet = fleet_init(
		sj_get_string_value(sj_object_get_value(fleet_json, "name")),
		status,
		sj_get_string_value(sj_object_get_value(fleet_json, "location"))
	);

	for (i = 0; i < max_ships; i++)
	{
			
		if (i < sj_array_get_count(ships_json))
		{
			fleet->ships[i] = *ship_fromJson(
				sj_array_get_nth(
					ships_json,
					i
				),
				fleet
			);
		}

		else { fleet->ships[i]._inuse = 0; }
	}

	fleet->_inuse = 1;

	return fleet;
}

Fleet* fleetlist_fromJson(SJson* fleets_json)
{
	int i;

	Fleet* fleet_list = NULL;

	if ((!fleets_json) || (!sj_is_array(fleets_json)))
	{
		slog("CANNOT LOAD FLEET LIST FROM THIS JSON"); return NULL;
	}

	fleet_list = malloc(sizeof(Fleet) * max_national_fleets);

	for (i = 0; i < max_national_fleets; i++)
	{

		if (i < sj_array_get_count(fleets_json))
		{

			fleet_list[i] = *fleet_fromjson(sj_array_get_nth(fleets_json, i));

			fleet_list[i]._inuse = 1;
		}

		else
		{
			fleet_list[i]._inuse = 0;
		}

		
	}

	return fleet_list;
}

Fleet* fleet_init(char name[128], int status, char location[128])
{
	//slog(name);
	int i = 0;

	Fleet* fleet = malloc(sizeof(Fleet));

	strcpy(fleet->name, name);

	strcpy(fleet->location, location);

	fleet->status = status;

	fleet->ships = malloc(sizeof(Ship) * max_ships);

	return fleet;
}

Menu_State* ship_menustate(Ship* ship, Menu_State* previous, Bool forconstruction, Game_Event* event_template)
{

	Menu_State* ship_menustate;

	UI_Element* costs_button;
	UI_Element* maintenance_button;
	UI_Element* construct_button;

	ship_menustate = menu_state_new(
		previous,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(300, 50),
			ship->type,
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	//Resource Cost
	costs_button = textbox_init
	(
		vector2d(10, 10),
		vector2d(200, 50),
		"Costs",
		font_load("resources/fonts/futur.ttf", 12)
	);

	costs_button->signal = new_gameevent(
		NULL,
		NULL,
		"GETCOSTS",
		NULL,
		0,
		resources_menustate_init(ship->costs, ship_menustate, "Building Costs"),
		0
	);

	menu_addTo
	(
		ship_menustate->current_menu,
		costs_button
	);
	
	//Maintenance Costs
	maintenance_button = textbox_init
	(
		vector2d(10, 10),
		vector2d(200, 50),
		"Maintenance Costs",
		font_load("resources/fonts/futur.ttf", 12)
	);

	maintenance_button->signal = new_gameevent(
		NULL,
		NULL,
		"GETMAINTENANCE",
		NULL,
		0,
		resources_menustate_init(ship->maintenance, ship_menustate, "Maintenance Costs"),
		0
	);

	menu_addTo
	(
		ship_menustate->current_menu,
		maintenance_button
	);
	
	//Construction button
	if (forconstruction)
	{
		construct_button = textbox_init
		(
			vector2d(10, 10),
			vector2d(200, 50),
			"Construct",
			font_load("resources/fonts/futur.ttf", 12)
		);

		construct_button->signal = malloc(sizeof(Game_Event));
		gameevent_copy(construct_button->signal, event_template);

		strcpy(construct_button->signal->descriptor, ship->type);

		menu_addTo(ship_menustate->current_menu, construct_button);
	}

	menu_state_hide(ship_menustate);

	return ship_menustate;

}

Menu_State* ships_menustate(Ship* ships, Menu_State* previous, Bool in_fleet, Game_Event* event_template)
{
	int i;

	char textbox_title[128];

	Menu_State* ships_menustate;

	UI_Element* ship_button;

	ships_menustate = menu_state_new(
		previous,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(300, 50),
			"Ships",
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);



	if (in_fleet == 1)
	{
		for (i = 0; i < max_ships; i++)
		{

			if (!ships[i]._inuse) { continue; }

			sprintf(textbox_title, "%s | %i | %s", ships[i].type, ships[i].health, ship_status_names[ships[i].status]);

			ship_button = textbox_init
			(
				vector2d(10, 10),
				vector2d(250, 50),
				textbox_title,
				font_load("resources/fonts/futur.ttf", 10)
			);

			menu_addTo(ships_menustate->current_menu, ship_button);
		}

	}

	else
	{
		for (i = 0; i < shipdict.numships; i++)
		{

			if (!shipdict.ship_templates[i]._inuse) { continue; }

			sprintf(textbox_title, "%s Ship", shipdict.ship_templates[i].type);

			ship_button = textbox_init
			(
				vector2d(10, 10),
				vector2d(250, 50),
				textbox_title,
				font_load("resources/fonts/futur.ttf", 12)
			);

			ship_button->signal = new_gameevent(
				shipdict.ship_templates[i].type,
				NULL,
				"DETAILS",
				NULL,
				0,
				ship_menustate(&shipdict.ship_templates[i], ships_menustate, 1, event_template),
				NULL
			);

			menu_addTo(ships_menustate->current_menu, ship_button);
		}
	}

	menu_state_hide(ships_menustate);

	return ships_menustate;
}

Menu_State* fleet_menustate(Fleet* fleet, Menu_State* previous)
{
	int i;

	Menu_State* fleet_menustate;

	UI_Element* all_ships_button;
	UI_Element* maintenancecosts_button;
	UI_Element* move_button;

	Game_Event* move_event;

	char temp0[128]; char temp1[128];

	float* temp_floats;

	fleet_menustate = menu_state_new(
		previous,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			fleet->name,
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	strcpy(temp1, ship_status_names[fleet->status]);

	sprintf(temp0, "%s | %s", fleet->location, temp1);

	menu_addTo(
		fleet_menustate->current_menu,

		textbox_init
		(
			vector2d(10, 10),
			vector2d(250, 50),
			temp0,
			font_load("resources/fonts/futur.ttf", 12)
		)
	);

	//Get Resources Total button
	maintenancecosts_button = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"Total Maintenance Costs",
		font_load("resources/fonts/futur.ttf", 12)
	);

	temp_floats = fleet_totalmaintenance(fleet);

	maintenancecosts_button->signal = new_gameevent(
		fleet->name,
		"MAINTENANCE",
		"SHOW_SUM",
		NULL,
		NULL,
		resources_menustate_init(temp_floats, fleet_menustate, "Maintenance Costs"),
		0
	);

	menu_addTo(fleet_menustate->current_menu, maintenancecosts_button);

	//Get Ships button
	all_ships_button = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"Ships",
		font_load("resources/fonts/futur.ttf", 12)
	);

	all_ships_button->signal = new_gameevent(
		fleet->name,
		"SHIPS",
		"SHOW_ALL",
		NULL,
		NULL,
		ships_menustate(fleet->ships, fleet_menustate, 1, NULL),
		0
	);

	menu_addTo(fleet_menustate->current_menu, all_ships_button);

	//Get move button

	move_button = textbox_init
	(
		vector2d(10, 10),
		vector2d(250, 50),
		"Move Fleet",
		font_load("resources/fonts/futur.ttf", 12)
	);

	move_event = new_gameevent(
		fleet->owner->name,
		fleet->name,
		"MOVETO",
		NULL,
		0,
		NULL,
		NULL
	);

	move_button->signal = new_gameevent(
		fleet->name,
		NULL,
		"CHOOSE_DESTINATION",
		NULL,
		NULL,
		system_neighbors_menustate(get_entity_by_name(fleet->location), fleet_menustate, "Move Fleet", move_event),
		0
	);
	
	//Menu_State* temp = move_button->signal->menu_state;
	//slog(temp->current_menu->beginning->element->signal->command);

	menu_addTo(fleet_menustate->current_menu, move_button);

	menu_state_hide(fleet_menustate);

	return fleet_menustate;
}

float* fleet_totalmaintenance(Fleet* fleet)
{
	int i;

	float* resources = malloc(numresources * sizeof(float));

	for (i = 0; i < numresources; i++) { resources[i] = 0; }

	for (i = 0; i < max_ships; i++)
	{
		if (!fleet->ships[i]._inuse) { continue; }

		resources = resourcelist_add(resources, fleet->ships[i].maintenance);
	}

	return resources;
}

int fleet_addShip(Fleet* fleet, Ship* ship)
{
	int i;

	if (!fleet)
	{
		slog("CANNOT ADD SHIP TO NULL FLEET"); return 0;
	}

	if (!ship)
	{
		slog("CANNOT ADD NULL SHIP TO FLEET"); return 0;
	}

	for (i = 0; i < max_ships; i++)
	{
		if (fleet->ships[i]._inuse != 1)
		{
			fleet->ships[i] = *ship;

			return 1;
		}
	}

	return -1;

}

int fleetlist_addFleet(Fleet* fleetlist, Fleet* fleet)
{

	int i;

	if (!fleetlist)
	{
		slog("CANNOT ADD FLEET TO NULL FLEET LIST"); return 0;
	}

	if (!fleet)
	{
		slog("CANNOT ADD NULL FLEET TO FLEET LIST"); return 0;
	}

	for (i = 0; i < max_national_fleets; i++)
	{

		if (fleetlist[i]._inuse == 1)
		{
			continue;
		}

		if (fleetlist[i]._inuse) { slog("%i", i); }

		fleetlist[i] = *fleet;
		fleetlist[i]._inuse = 1;

		return 1;
	}

	return -1;

}

void ship_free(Ship* self)
{

}

SJson* ship_toJson(Ship* self)
{
	SJson* ship = sj_object_new();

	sj_object_insert(ship, "type", sj_new_str(self->type));

	sj_object_insert(ship, "status", sj_new_int(self->status));

	sj_object_insert(ship, "health", sj_new_int(self->health));

	sj_object_insert(ship, "maintenance", resources_toJson(self->maintenance));

	return ship;
}

void fleet_free(Fleet* self)
{

}

SJson* fleet_toJson(Fleet* self)
{
	int i;
	SJson* fleet_json = sj_object_new();
	SJson* ships_json = sj_array_new();

	sj_object_insert(fleet_json, "name", sj_new_str(self->name));

	sj_object_insert(fleet_json, "status", sj_new_int(self->status));

	sj_object_insert(fleet_json, "location", sj_new_str(self->location));

	for (i = 0; i < max_ships; i++)
	{
		if (!self->ships[i]._inuse) { continue; }

		sj_array_append(ships_json, ship_toJson(&self->ships[i]));
	}

	if (sj_array_get_count(ships_json) > 0) { sj_object_insert(fleet_json, "ships", ships_json); }

	return fleet_json;
}

SJson* fleetlist_toJson(Fleet* fleetlist)
{
	int i;

	SJson* fleetlist_json = sj_array_new();

	for (i = 0; i < max_national_fleets; i++)
	{
		if (fleetlist[i]._inuse != 1) { continue; }

		sj_array_append(
			fleetlist_json,
			fleet_toJson(&fleetlist[i])
		);
	}

	return fleetlist_json;
}