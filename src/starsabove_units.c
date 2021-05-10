#include "starsabove_units.h"

#include "simple_logger.h"

#include "starsabove_nation.h"
#include "starsabove_system.h"
#include "starsabove_game_resources.h"

//#include "starsabove_unit_marine.h"
//#include "starsabove_unit_spy.h"
//#include "starsabove_unit_colonist.h"

Unit* unit_fromJson(SJson* unit_json)
{
	int type;
	int health;
	int status;
	int buildtime;

	Unit* new_unit;

	if (!unit_json)
	{
		slog("CANNOT CREATE UNIT FROM NULL JSON"); return;
	}

	sj_get_integer_value(sj_object_get_value(unit_json, "type"), type);
	sj_get_integer_value(sj_object_get_value(unit_json, "health"), health);
	sj_get_integer_value(sj_object_get_value(unit_json, "status"), status);
	sj_get_integer_value(sj_object_get_value(unit_json, "buildtime"), buildtime);

	new_unit = unit_new
	(
		type,
		health,
		status,
		buildtime,
		resources_fromJson(sj_object_get_value(unit_json, "maintenance")),
		resources_fromJson(sj_object_get_value(unit_json, "costs"))
	);

	strcpy(new_unit->location, sj_get_string_value(unit_json, "location"));
	new_unit->locationtype = LOCATION_STRING;

	return new_unit;

}

Unit* unit_new(int type, int health, int status, int buildtime, float* maintenance, float* costs)
{
	Unit* new_unit;

	new_unit = unit_init();

	new_unit->type = (unit_type)type;

	new_unit->health = health;
	new_unit->status = status;
	new_unit->buildtime = buildtime;

	new_unit->maintenance = resourcelist_copy(maintenance);
	new_unit->costs = resourcelist_copy(costs);

	new_unit->_inuse = 1;

	return new_unit;
}

Unit* unit_init()
{
	return malloc(sizeof(Unit));
}

Menu_State* unit_menustate(Unit* self, Menu_State* previous, Bool forconstruction)
{

}

float* unit_onNewTurn(Unit* self)
{

}

SJson* unit_toJson(Unit* self)
{
	SJson* unit_json;

	if (!self)
	{
		slog("CANNOT TURN NULL UNIT TO JSON"); return;
	}

	unit_json = sj_object_new();

	sj_object_insert(unit_json, "type", sj_new_int((int)self->type));

	sj_object_insert(unit_json, "health", sj_new_int(self->health));
	sj_object_insert(unit_json, "status", sj_new_int(self->status));

	sj_object_insert(unit_json, "buildtime", sj_new_int(self->buildtime));

	sj_object_insert(unit_json, "maintenance", resources_toJson(self->maintenance));
	sj_object_insert(unit_json, "costs", resources_toJson(self->costs));

	sj_object_insert(unit_json, "unitdata", self->toJson(self));

	return unit_json;
}

Unit* unit_free()
{

}