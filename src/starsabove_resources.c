#include "starsabove_resources.h"

char resource_names[6][128] =
{
	"Food",
	"Iron",
	"Uranium",
	"Gold",
	"Silicon",
	"Plastoil"
};

float* resources_fromJson(SJson* resources_json)
{
	float food = 0; 
	float iron = 0;
	float uranium = 0;
	float gold = 0;
	float silicon = 0;
	float plastoil = 0;

	sj_get_float_value(sj_object_get_value(resources_json, "Food"), &food);
	sj_get_float_value(sj_object_get_value(resources_json, "Iron"), &iron);
	sj_get_float_value(sj_object_get_value(resources_json, "Uranium"), &uranium);
	sj_get_float_value(sj_object_get_value(resources_json, "Gold"), &gold);
	sj_get_float_value(sj_object_get_value(resources_json, "Silicon"), &silicon);
	sj_get_float_value(sj_object_get_value(resources_json, "Plastoil"), &plastoil);

	return resourcelist_new
	(
		food,
		iron,
		uranium,
		gold,
		silicon,
		plastoil
	);
}

SJson* resources_toJson(float* resources)
{
	int i;
	SJson* resources_json = sj_object_new(); 

	/*sj_object_insert(resources_json, "food", sj_new_float(resources[RES_FOOD]));
	sj_object_insert(resources_json, "iron", sj_new_float(resources[RES_IRON]));
	sj_object_insert(resources_json, "uranium", sj_new_float(resources[RES_URANIUM]));
	sj_object_insert(resources_json, "gold", sj_new_float(resources[RES_GOLD]));
	sj_object_insert(resources_json, "silicon", sj_new_float(resources[RES_SILICON]));
	sj_object_insert(resources_json, "plastoil", sj_new_float(resources[RES_PLASTOIL]));*/

	for (i = 0; i < numresources; i++)
	{
		sj_object_insert(resources_json, resource_names[i], sj_new_float(resources[i]));
	}

	return resources_json;
}

Menu_State* resources_menustate_init(float* resources, Menu_State* previous_menustate, char* name)
{
	int i;

	char textbox_name[128];

	Menu_State* resources_menustate;

	//Create the planet menu state
	resources_menustate = menu_state_new(
		previous_menustate,
		textbox_init
		(
			vector2d(10, 10),
			vector2d(200, 50),
			name,
			font_load("resources/fonts/futur.ttf", 16)
		),
		NULL,
		vector2d(10, 10),
		0,
		5
	);

	for (i = 0; i < numresources; i++)
	{

		sprintf(textbox_name, "%s: %.3f", resource_names + i, resources[i]);

		menu_addTo
		(
			resources_menustate->current_menu,
			textbox_init
			(
				vector2d(10, 10),
				vector2d(200, 50),
				textbox_name,
				font_load("resources/fonts/futur.ttf", 12)
			)
		);

		strcpy(textbox_name, "");

	}

	//Hide the planet_menustate
	menu_state_hide(resources_menustate);

	return resources_menustate;
}

float* resourcelist_new(float food, float iron, float uranium, float gold, float silicon, float plastoil)
{
	float* resources = malloc(numresources * sizeof(float));

	resources[RES_FOOD] = food;
	resources[RES_IRON] = iron;
	resources[RES_URANIUM] = uranium;
	resources[RES_GOLD] = gold;
	resources[RES_SILICON] = silicon;
	resources[RES_PLASTOIL] = plastoil;

	return resources;
}

float* resourcelist_add(float* arr0, float* arr1)
{
	int i;

	float* sum = malloc(numresources * sizeof(float));

	for (i = 0; i < numresources; i++)
	{
		sum[i] = arr0[i] + arr1[i];
	}

	return sum;
}

float* resourcelist_subtract(float* arr0, float* arr1)
{
	int i;

	float* difference = malloc(numresources * sizeof(float));

	for (i = 0; i < numresources; i++)
	{
		difference[i] = arr0[i] - arr1[i];
	}

	return difference;
}

Bool resourcelist_checkdeficit(float* arr)
{
	int i;

	for (i = 0; i < numresources; i++)
	{
		if (arr[i] < 0)
		{
			return 0;
		}
	}

	return 1;

}

float* resourcelist_copy(float* arr)
{
	int i;

	float* copy = malloc(numresources * sizeof(float));

	for (i = 0; i < numresources; i++)
	{
		copy[i] = arr[i];
	}

	return copy;
}