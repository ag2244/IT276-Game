#pragma once

#ifndef __RESOURCES_H__
#define __RESOURCES_H__

#include "simple_logger.h"
#include "simple_json.h"

#include "gfc_types.h"

#include "starsabove_ui_textbox.h"

static const int numresources = 6; //Number of resources there are

enum resources
{
	RES_FOOD = 0,
	RES_IRON,
	RES_URANIUM,
	RES_GOLD,
	RES_SILICON,
	RES_PLASTOIL
};

/*
* @brief Load a resources list from an SJSon
* @param resources_json The json resources array to load
* @return NULL on error, or an array of floats
*/
float* resources_fromJson(SJson* resources_json);

/*
* @brief Save resources array to an SJson object
* @param resources The resources array to save
* @return NULL on error, or an SJson object
*/
SJson* resources_toJson(float* resources);

/*
* @brief Make a menustate for a list of resources
* @param resources The array of resources
* @param planet_menustate The previous menustate to the one that will be returned
* @param name The name of the menu
* @param excluse_zeroes Exclude resources that have a value of zeros
* @return NULL on error, or a pointer to a new Menu_State
*/
Menu_State* resources_menustate_init(float* resources, Menu_State* previous_menustate, char* name, Bool exclude_zeroes);

/*
* @brief Make a menustate for two lists of resources (resources out of a certain number)
* @param resources The array of resources
* @param outof The array of resources out of which resources is
* @param planet_menustate The previous menustate to the one that will be returned
* @param name The name of the menu
* @return NULL on error, or a pointer to a new Menu_State
*/
Menu_State* resources_comparative_menustate_init(float* resources, float* outof, Menu_State* previous_menustate, char* name);

/*
* @brief Get new array of resources
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_new(float food, float iron, float uranium, float gold, float silicon, float plastoil);


/*
* @brief Get new array of resources, all with value 0
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_empty();

/*
* @brief Add two resource arrays
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_add(float* arr0, float* arr1);

/*
* @brief Subtract one resource array from another
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_subtract(float* arr0, float* arr1);

/*
* @brief Multiply two resource arrays
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_multiply(float* arr0, float* arr1);

/*
* @brief Check if a resource list has any negatives
* @return NULL on error, or a 0 if arr contains a negative, or 1 if it doesnt
*/
Bool resourcelist_checkdeficit(float* arr);

/*
* @brief Check if a resource list is all zeroes.
* @return NULL on error, or a 0 if arr contains a nonzero, or 1 if it doesnt
*/
Bool resourcelist_iszero(float* arr);

/*
* @brief Copy one resource array to another new one
* @return NULL on error, or an array of floats, with keys being the enum values in resources
*/
float* resourcelist_copy(float* arr);

/*
* @brief Print out the contents of a resource array
*/
void resourcelist_print(float* arr);

#endif