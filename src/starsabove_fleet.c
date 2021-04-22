#include "simple_logger.h"

#include "starsabove_fleet.h"

#include "starsabove_resources.h"

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

		shipdict.ship_templates[i].status = SHIP_TEMPLATE;
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

}

Ship* ship_copy(Ship* src, Fleet* fleet)
{
	return ship_init(src->type, src->maintenance, fleet);
}

Ship* ship_fromJson(SJson* ship_json, Fleet* fleet)
{

	if (!ship_json)
	{
		slog("CANNOT LOAD NULL JSON SHIP"); return NULL;
	}

	return ship_init(
		sj_get_string_value(sj_object_get_value(ship_json, "type")),
		resourcelist_copy(resources_fromJson(sj_object_get_value(ship_json, "maintenance"))),
		fleet);
}

Ship* ship_init(char shiptype[128], float* maintenance, Fleet* fleet)
{
	Ship* ship = malloc(sizeof(Ship));

	strcpy(ship->type, shiptype);
	ship->fleet = fleet;

	ship->maintenance = resourcelist_copy(maintenance);

	return ship;
}

Fleet* fleet_fromjson(SJson* fleet_json)
{
	
}

Fleet* fleet_init(char name[128])
{

}

Menu_State* fleet_menustate(Fleet* fleet)
{

}

void ship_free(Ship* self)
{

}

SJson* ship_toJson(Ship* self)
{

}

void fleet_free(Fleet* self)
{

}

SJson* fleet_toJson(Fleet* self)
{

}