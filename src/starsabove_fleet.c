#include "simple_logger.h"

#include "starsabove_fleet.h"

#include "starsabove_nation.h"

typedef struct
{
	int numships;
	Ship* ship_templates;
} 
Ship_Dict;

Ship_Dict shipdict = { 0 };

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
}

Ship* ship_copy(Ship* src, Fleet* fleet)
{
	return ship_init(src->type, src->maintenance, src->health, src->status, fleet);
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
		health,
		status,
		fleet);
}

Ship* ship_init(char shiptype[128], float* maintenance, int health, int status, Fleet* fleet)
{
	Ship* ship = malloc(sizeof(Ship));

	strcpy(ship->type, shiptype);
	ship->fleet = fleet;

	ship->maintenance = resourcelist_copy(maintenance);

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

	if (!fleet_json)
	{
		slog("CANNOT LOAD FLEET FROM NULL JSON"); return NULL;
	}

	ships_json = sj_object_get_value(fleet_json, "ships");

	if (!ships_json)
	{
		slog("CANNOT LOAD FLEET SHIPS FROM NULL JSON"); return NULL;
	}

	fleet = fleet_init(
		sj_get_string_value(sj_object_get_value(fleet_json, "name")),
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

	for (i = 0; i < sj_array_get_count(fleets_json); i++)
	{
		fleet_list[i] = *fleet_fromjson(sj_array_get_nth(fleets_json, i));

		fleet_list[i]._inuse = 1;
	}

	return fleet_list;
}

Fleet* fleet_init(char name[128], Entity* location)
{
	int i = 0;

	Fleet* fleet = malloc(sizeof(Fleet));

	strcpy(fleet->name, name);

	strcpy(fleet->location, location);

	fleet->ships = malloc(sizeof(Ship) * max_ships);

	return fleet;
}

Menu_State* fleet_menustate(Fleet* fleet)
{

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