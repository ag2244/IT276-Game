#pragma once

#include "simple_json.h"

#include "starsabove_ui_textbox.h"

typedef struct Army;

static const int num_division_types = 5;
static const int max_divisions = 30;

static enum division_type
{
	ARMY_INFANTRY,
	ARMY_ARTILLERY,
	ARMY_MECHANIZED,
	ARMY_ENGINEER,
	ARMY_AIR
};

static char division_names[5][128] =
{
	"Infantry",
	"Artillery",
	"Mechanized",
	"Engineer",
	"Air"
};

typedef struct
{

	char name[128];

	division_type type;

	float* costs;

	Army* army;

} Division;

typedef struct
{

	char name[128];
	
	Division* divisions;

} Army;

Division* division_fromJson();

Division* division_init();

Army* army_fromjson();

Army* army_init();

Menu_State* army_menustate();

void division_free(Division* self);

SJson* division_toJson(Division* self);

void army_free(Army* self);

SJson* army_toJson(Army* self);