#include "starsabove_resources.h"

float* resources_fromJson(SJson* resources_json)
{
	float* food = 0; 
	float* iron = 0;
	float* uranium = 0;
	float* gold = 0;
	float* silicon = 0;
	float* plastoil = 0;

	sj_get_float_value(sj_array_get_nth(resources_json, RES_FOOD), food);
	sj_get_float_value(sj_array_get_nth(resources_json, RES_IRON), iron);
	sj_get_float_value(sj_array_get_nth(resources_json, RES_URANIUM), uranium);
	sj_get_float_value(sj_array_get_nth(resources_json, RES_GOLD), gold);
	sj_get_float_value(sj_array_get_nth(resources_json, RES_SILICON), silicon);
	sj_get_float_value(sj_array_get_nth(resources_json, RES_PLASTOIL), plastoil);

	return resourcelist_new
	(
		*food,
		*iron,
		*uranium,
		*gold,
		*silicon,
		*plastoil
	);
}

SJson* resources_toJson(float* resources)
{
	SJson* resources_json = sj_object_new(); 

	sj_object_insert(resources_json, "food", sj_new_float(resources[RES_FOOD]));
	sj_object_insert(resources_json, "iron", sj_new_float(resources[RES_IRON]));
	sj_object_insert(resources_json, "uranium", sj_new_float(resources[RES_URANIUM]));
	sj_object_insert(resources_json, "gold", sj_new_float(resources[RES_GOLD]));
	sj_object_insert(resources_json, "silicon", sj_new_float(resources[RES_SILICON]));
	sj_object_insert(resources_json, "plastoil", sj_new_float(resources[RES_PLASTOIL]));

	return resources_json;
}

float* resourcelist_new(float food, float iron, float uranium, float gold, float silicon, float plastoil)
{
	float resources[6];

	resources[RES_FOOD] = food;
	resources[RES_IRON] = iron;
	resources[RES_URANIUM] = uranium;
	resources[RES_GOLD] = gold;
	resources[RES_SILICON] = silicon;
	resources[RES_PLASTOIL] = plastoil;

	return resources;
}