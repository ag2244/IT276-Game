#include <stdlib.h>
#include "gfc_types.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "starsabove_entity.h"

Collider_Box box_new(Collider_Box* box, float in_width, float in_height, Vector2D in_position)
{

	if (box == NULL) box = malloc(sizeof(Collider_Box));

	box->width = in_width;
	box->height = in_height;
	vector2d_copy(box->position, in_position);

}

Collider_Circle circle_new(Collider_Circle* circle, float in_radius, Vector2D in_position)
{
	if (circle == NULL) circle = malloc(sizeof(Collider_Circle));

	circle->radius = in_radius;
	vector2d_copy(circle->position, in_position);
}

void box_free(Collider_Box* box) 
{
	free(box);
}

void circle_free(struct Collider_Circle* circle) {

	free(circle);

}

Bool box_clickable(struct Collider_Circle_s self, Vector2D otherPosition) {

}

Bool circle_clickable(struct Collider_Circle_s self, Vector2D otherPosition) {

}
