#include <stdlib.h>
#include <math.h>

#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "starsabove_collision.h"

Collider_Box* box_new(Collider_Box* box, Vector2D in_position, Vector2D in_extremity)
{

	return malloc(sizeof(Collider_Box));

}

Collider_Circle* circle_new()
{
	return malloc(sizeof(Collider_Circle));
}

void box_free(Collider_Box* box) 
{

	if (box == NULL)
	{
		slog("Cannot free NULL box!");
		return NULL;
	}

	free(box);
}

void circle_free(Collider_Circle* circle) {

	if (circle == NULL)
	{
		slog("Cannot free NULL circle!");
		return NULL;
	}

	free(circle);
}

Bool box_clickable(Collider_Box* box, Vector2D otherPosition) {

	if (box == NULL)
	{
		slog("Cannot check if NULL circle is clickable");
		return NULL;
	}

	if (
		(otherPosition.x > box->position.x) && (otherPosition.y > box->position.y) 
		&& 
		(otherPosition.x < box->position.x + box->size.x) && (otherPosition.y < box->position.y + box->size.y))
	{
		return 1;
	}

	return 0;
}

Bool circle_clickable(Collider_Circle* circle, Vector2D otherPosition) 
{

	if (circle == NULL) 
	{
		slog("Cannot check if NULL circle is clickable");
		return NULL;
	}

	/*if (otherPosition)
	{
		slog("Circle cannot be compared to NULL point");
		return NULL;
	}*/

	if (circle->radius == 0)
	{
		slog("Radius cannot be 0");
		return NULL;
	}
	
	return vector2d_distance_between_less_than(circle->position, otherPosition, circle->radius);
}
