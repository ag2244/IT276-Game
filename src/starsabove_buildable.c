#include "simple_logger.h"

#include "starsabove_buildable.h"

Buildable* buildable_fromJson(SJson* buildable_json)
{
	char* name;
	int* status;

	strcpy(name, sj_get_string_value(sj_object_get_value(buildable_json, "name")));

	sj_get_float_value(sj_object_get_value(buildable_json, "status"), status);

	return buildable_new(
		status,
		name,
		resources_fromJson(sj_object_get_value(buildable_json, "input")),
		resources_fromJson(sj_object_get_value(buildable_json, "output"))
	);

}

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

void buildable_free(Buildable* buildable)
{
	free(buildable);
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