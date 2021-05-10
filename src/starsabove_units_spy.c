#include "starsabove_units_spy.h"

Spy* spyfromlist(Spy* spylist, int i)
{
	
	if (spylist[i]._inuse == 1)
	{
		return &spylist[i];
	}

	return NULL;
}

Spy* spy_copy(Spy* self)
{

}

float* spy_onNewTurn(Spy* self)
{

}

Menu_State* spy_menustate(Spy* self, Menu_State* previous, Bool forconstruction)
{
	return NULL;
}

Menu_State* spies_menustate(Spy* spies, int numspies, Menu_State* previous, Bool forconstruction, Bool inlocation, char location[128], char sublocation[128])
{

}

Spy* spy_fromJson(SJson* unit_json)
{
	int* status = 0;
	int* buildtime = 0;

	if (!unit_json)
	{
		slog("CANNOT BUILD SPY FROM NULL JSON"); return NULL;
	}

	sj_get_integer_value(sj_object_get_value(unit_json, "status"), &status);
	sj_get_integer_value(sj_object_get_value(unit_json, "buildtime"), &buildtime);

	return spy_init(
		status,
		buildtime,
		resources_fromJson(sj_object_get_value(unit_json, "maintenance")),
		resources_fromJson(sj_object_get_value(unit_json, "costs")),
		sj_get_string_value(sj_object_get_value(unit_json, "location")),
		sj_get_string_value(sj_object_get_value(unit_json, "sublocation"))
	);
}

Spy* spies_fromJson(SJson* spies_array)
{
	int i;
	Spy* spies = NULL;

	if (!spies_array)
	{
		slog("CANNOT BUILD SPIES FROM NULL JSON"); return NULL;
	}

	if (!sj_is_array(spies_array))
	{
		slog("CANNOT BUILD SPIES FROM NON ARRAY JSON"); return NULL;
	}

	spies = malloc(sizeof(Spy) * max_national_spies);

	for (i = 0; i < max_national_spies; i++)
	{
		
		if (i < sj_array_get_count(spies_array)) { spies[i] = *spy_fromJson(sj_array_get_nth(spies_array, i)); }

		else { spies[i]._inuse = 0; }
	}

	return spies;
}

Spy* spy_init(int status, int buildtime, float* maintenance, float* costs, char location[128], char sublocation[128])
{
	Spy* spy = malloc(sizeof(Spy));

	spy->status = status;
	spy->buildtime = buildtime;

	spy->maintenance = resourcelist_copy(maintenance);
	spy->costs = resourcelist_copy(costs);

	strcpy(spy->location, location);
	strcpy(spy->sublocation, sublocation);

	spy->_inuse = 1;

	return spy;
}

SJson* spy_toJson(Spy* self)
{
	SJson* spy_json;

	SJson* maintenance;
	SJson* costs;

	if (!self)
	{
		slog("CANNOT TURN NULL SPY INTO JSON"); return;
	}

	spy_json = sj_object_new();

	sj_object_insert(spy_json, "status", sj_new_int(self->status));

	if (self->buildtime != 0) { sj_object_insert(spy_json, "buildtime", sj_new_int(self->buildtime)); }

	maintenance = resources_toJson(self->maintenance);
	if (sj_array_get_count(maintenance) > 0) { sj_object_insert(spy_json, "maintenance", maintenance); }

	costs = resources_toJson(self->costs);
	if (sj_array_get_count(costs) > 0) { sj_object_insert(spy_json, "costs", costs); }

	sj_object_insert(spy_json, "location", sj_new_str(self->location));

	sj_object_insert(spy_json, "sublocation", sj_new_str(self->sublocation));

	return spy_json;
}

SJson* spies_toJson(Spy* spies_array)
{

	int i;

	SJson* spies_json = sj_array_new();

	for (i = 0; i < max_national_spies; i++)
	{
		if (spies_array[i]._inuse != 1) { continue; }

		sj_array_append(
			spies_json,
			spy_toJson(&spies_array[i])
		);
	}

	return spies_json;
}

void spy_free(Spy* self)
{

}