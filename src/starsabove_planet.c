#include "gfc_types.h"

#include "starsabove_planet.h"

Planet* planet_fromJson(SJson* planetjson) {

    float* temp;

	Planet* planet = planet_new(
		sj_get_string_value(sj_object_get_value(planetjson, "name")), 
        resources_fromJson(sj_object_get_value(planetjson, "resources"))
	);

    temp = resources_fromJson(sj_object_get_value(planetjson, "resources"));

    planet->resources_mining = malloc(6 * sizeof(float));

    for (int i = 0; i < 6; i++)
    {
        planet->resources_mining[i] = temp[i];
    }

    free(temp);

	return planet;

}

SJson* planet_toJson(Planet* planet)
{
	SJson* planet_json = sj_object_new();

	sj_object_insert(planet_json, "name", sj_new_str(planet->name));

    sj_object_insert(planet_json, "resources", resources_toJson(planet->resources_mining));

    return planet_json;
}

Menu_State* planet_menustate_init(Planet* planet, Menu_State* system_menustate, char* system_name, Bool playerOwned)
{
    Menu_State* planet_menustate;

    UI_Element* buildings_button;
    UI_Element* construction_button;

    UI_Element* resources_button;
    char resource_button_name[128];

    if (planet == NULL)
    {
        slog("Cannot initialize planet Menu_State for NULL planet"); return NULL;
    }

    if (system_menustate == NULL)
    {
        slog("Cannot initialize planet Menu_State to add to NULL system_menustate"); return NULL;
    }

    if (!system_name)
    {
        slog("Cannot initialize planet Menu_State with NULL system_name"); return NULL;
    }

    //Create the planet menu state
    planet_menustate = menu_state_new(
        system_menustate,
        textbox_init
        (
            vector2d(10, 10),
            vector2d(200, 50),
            planet->name,
            font_load("resources/fonts/futur.ttf", 16)
        ),
        NULL,
        vector2d(10, 10),
        0,
        5
    );

    //Create resources button
    resources_button = textbox_init
    (
        vector2d(10, 10),
        vector2d(200, 50),
        "Resources",
        font_load("resources/fonts/futur.ttf", 12)
    );

    sprintf(resource_button_name, "%s Resources", planet->name);

    resources_button->signal = new_gameevent(
        system_name,
        planet->name,
        "GETRESOURCES",
        NULL,
        0,
        resources_menustate_init(planet->resources_mining, planet_menustate, resource_button_name),
        0
    );

    menu_addTo
    (
        planet_menustate->current_menu,
        resources_button
    );

    if (playerOwned)
    {
        //Create buildings button
        buildings_button = textbox_init
        (
            vector2d(10, 10),
            vector2d(200, 50),
            "Buildings",
            font_load("resources/fonts/futur.ttf", 12)
        );

        buildings_button->signal = new_gameevent(
            system_name,
            planet->name,
            "GETBUILDINGS",
            NULL,
            0,
            NULL, //resources_menustate_init(planet->resources_mining, planet_menustate, resource_button_name),
            0
        );

        menu_addTo
        (
            planet_menustate->current_menu,
            buildings_button
        );

        //Create construction button
        construction_button = textbox_init
        (
            vector2d(10, 10),
            vector2d(200, 50),
            "Construction",
            font_load("resources/fonts/futur.ttf", 12)
        );

        construction_button->signal = new_gameevent(
            system_name,
            planet->name,
            "CONSTRUCTION",
            NULL,
            0,
            NULL, //resources_menustate_init(planet->resources_mining, planet_menustate, resource_button_name),
            0
        );

        menu_addTo
        (
            planet_menustate->current_menu,
            construction_button
        );
    }

    //Hide the planet_menustate
    menu_state_hide(planet_menustate);

    return planet_menustate;
}

Planet* planet_new(char* name, float* resource_arr)
{
	Planet* planet = malloc(sizeof(Planet));

	strcpy(planet->name, name);

	return planet;
}