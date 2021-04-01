#include "simple_logger.h"

#include "starsabove_buildable.h"

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
	}

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

	strcpy(name, sj_get_string_value(sj_object_get_value(buildable_json, "name")));

	if (sj_object_get_value(buildable_json, "status"))
	{
		sj_echo(sj_object_get_value(buildable_json, "status"));
		sj_get_integer_value(sj_object_get_value(buildable_json, "status"), &status);
	}

	return buildable_new(
		status,
		name,
		resources_fromJson(sj_object_get_value(buildable_json, "input")),
		resources_fromJson(sj_object_get_value(buildable_json, "output"))
	);

}

SJson* buildable_toJson(Buildable* buildable)
{
	int i = 0;
	SJson* buildable_json = sj_object_new();

	sj_object_insert(buildable_json, "name", sj_new_str(buildable->name));

	sj_object_insert(buildable_json, "input", resources_toJson(buildable->resource_input));
	sj_object_insert(buildable_json, "output", resources_toJson(buildable->resource_output));

	return buildable_json;
}


/* Create, copy and destroy buildables */
Buildable* buildable_new(int status, char* name, float* input, float* output)
{
	int i;

	Buildable* buildable = malloc(sizeof(Buildable));

	buildable->status = status;
	strcpy(buildable->name, name);

	buildable->resource_input = malloc(6 * sizeof(float));
	buildable->resource_output = malloc(6 * sizeof(float));;

	for (i = 0; i < 6; i++)
	{
		buildable->resource_input[i] = 0;
		buildable->resource_output[i] = 0;
	}

	for (i = 0; i < 6; i++)
	{
		buildable->resource_input[i] = input[i];
		buildable->resource_output[i] = input[i];
	}

	return buildable;
}

void buildable_copy(Buildable* dst, Buildable* src)
{
	slog(src->name);

	dst = buildable_new(src->status, src->name, src->resource_input, src->resource_output);

	slog(dst->name);
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

	if (!buildable)
	{
		slog("CANNOT MAKE MENUSTATE FOR NULL BUILDING"); return;
	}

	building_menustate = menu_state_new
	(
		previous_menustate,
		textbox_init(
			vector2d(10, 10),
			vector2d(200, 50),
			buildable->name,
			font_load("resources/fonts/futur.ttf", 14)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	menu_addTo
	(
		building_menustate->current_menu,
		textbox_init(
			vector2d(10, 10),
			vector2d(200, 50),
			"TEST",
			font_load("resources/fonts/futur.ttf", 14)
		)
	);

	menu_state_hide(building_menustate);

	return building_menustate;

}