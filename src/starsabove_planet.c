#include "gfc_types.h"

#include "starsabove_planet.h"

void planet_fromJson_buildings(SJson* planetjson, Planet* planet)
{
    int i;

    SJson* buildings = sj_object_get_value(planetjson, "buildings");

    if (buildings) 
    { 
        planet->num_buildings = sj_array_get_count(buildings);

        planet->buildings = malloc(num_ingame_buildables() * sizeof(Buildable));

        for (i = 0; i < planet->num_buildings; i++)
        {
            planet->buildings[i] = *buildable_fromJson(sj_array_get_nth(buildings, i));
        }

    }

    else { planet->buildings = NULL; planet->num_buildings = 0; }

}

Planet* planet_fromJson(SJson* planetjson) {

    float* temp;

	Planet* planet = planet_new(
		sj_get_string_value(sj_object_get_value(planetjson, "name")), 
        resources_fromJson(sj_object_get_value(planetjson, "resources"))
	);

    //Load buildings
    planet_fromJson_buildings(planetjson, planet);

    //Load resources
    temp = resources_fromJson(sj_object_get_value(planetjson, "resources"));

    planet->resources_mining = malloc(numresources * sizeof(float));

    for (int i = 0; i < numresources; i++)
    {
        planet->resources_mining[i] = temp[i];
    }

    free(temp);

	return planet;

}

SJson* planet_toJson(Planet* planet)
{
    int i;

	SJson* planet_json = sj_object_new();
    SJson* buildings = sj_array_new();

	sj_object_insert(planet_json, "name", sj_new_str(planet->name));

    sj_object_insert(planet_json, "resources", resources_toJson(planet->resources_mining));

    for (i = 0; i < planet->num_buildings; i++)
    { 
        sj_array_append(buildings, buildable_toJson(&planet->buildings[i]));
    }

    if (planet->num_buildings) { sj_object_insert(planet_json, "buildings", buildings); }

    return planet_json;
}

//Create the planet's menu state
UI_Element* planet_menustate_buildingsbutton(Planet* planet, Menu_State* planet_menustate, char* system_name)
{
    int i;

    UI_Element* buildings_button;
    UI_Element* building_option;

    Menu_State* buildings_menu = NULL;

    char buildings_menu_title[128] = "";

    sprintf(buildings_menu_title, "%s Buildings", planet->name);

    buildings_menu = menu_state_new(
        planet_menustate,
        textbox_init(
            vector2d(10, 10),
            vector2d(200, 50),
            buildings_menu_title,
            font_load("resources/fonts/futur.ttf", 14)
        ),
        NULL,
        vector2d(10, 10),
        0,
        5
    );
    
    for (i = 0; i < planet->num_buildings; i++) 
    {

        building_option = textbox_init
        (
            vector2d(10, 10),
            vector2d(200, 50),
            planet->buildings[i].name,
            font_load("resources/fonts/futur.ttf", 12)
        );

        building_option->signal = new_gameevent
        (
            system_name,
            planet->name,
            "SHOW ALL BUILDINGS",
            NULL,
            NULL,
            buildable_menustate_init(&planet->buildings[i], planet_menustate),
            0
        );

        menu_addTo(
            buildings_menu->current_menu,
            building_option
        );
    }
    
    menu_state_hide(buildings_menu);

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
        buildings_menu,
        0
    );

    return buildings_button;
}

UI_Element* planet_menustate_constructionbutton(Planet* planet, Menu_State* planet_menustate, char* system_name)
{
    UI_Element* construction_button;

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

    return construction_button;
}

Menu_State* planet_menustate_init(Planet* planet, Menu_State* system_menustate, char* system_name, Bool playerOwned)
{
    Menu_State* planet_menustate;

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
        if (planet->num_buildings > 0)
        {
            menu_addTo
            (
                planet_menustate->current_menu,
                planet_menustate_buildingsbutton(planet, planet_menustate, system_name)
            );
        }

        /*menu_addTo
        (
            planet_menustate->current_menu,
            planet_menustate_constructionbutton(planet, planet_menustate, system_name)
        );*/
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