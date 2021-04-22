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

Bool box_detectCollision(Collider_Box* box0, Collider_Box* box1)
{

	if ((box0 == NULL) || (box1 == NULL))
	{
		slog("Cannot detect collision with a NULL box"); return 0;
	}

	if (
		(box0->position.x + box0->size.x > box1->position.x) &&		//R0 bottom right x > R1 top left x
		(box0->position.x < box1->position.x + box1->size.x) &&		//R0 top left x > R1 bottom right x
		(box0->position.y + box0->size.y > box1->position.y) &&		//R0 bottom right y > R1 top left y
		(box0->position.y < box1->position.y + box1->size.y)		//R0 top left y > R1 bottom right y
		)
	{
		return 1;
	}

	else if (
		(box0->position.x + box0->size.x > box1->position.x) &&		//R1 bottom right x > R0 top left x
		(box0->position.x < box1->position.x + box1->size.x) &&		//R1 top left x > R0 bottom right x
		(box0->position.y + box0->size.y > box1->position.y) &&		//R1 bottom right y > R0 top left y
		(box0->position.y < box1->position.y + box1->size.y)		//R1 top left y > R0 bottom right y
		)
	{
		return 1;
	}

	return 0;

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
		(otherPosition.x > box->viewpos.x) && (otherPosition.y > box->viewpos.y)
		&& 
		(otherPosition.x < box->viewpos.x + box->size.x) && (otherPosition.y < box->viewpos.y + box->size.y))
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
	
	return vector2d_distance_between_less_than(circle->viewpos, otherPosition, circle->radius);
}
