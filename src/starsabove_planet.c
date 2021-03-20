#include "gfc_types.h"

#include "starsabove_planet.h"

Planet* planet_fromJson(SJson* planetjson) {

	Planet* planet = planet_new(
		sj_get_string_value(sj_object_get_value(planetjson, "name"))
	);

	return planet;

}

Planet* planet_new(char* name)
{
	Planet* planet = malloc(sizeof(Planet));

	strcpy(planet->name, name);

	return planet;
}