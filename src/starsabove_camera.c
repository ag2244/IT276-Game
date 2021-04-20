#include "starsabove_camera.h"

#include "simple_logger.h"

Camera game_cam = { 0 };

Camera* camera_get()
{
	return &game_cam;
}

Camera* camera_init(Vector2D pos, Vector2D size, Vector2D movementspeed)
{

	if (&pos == NULL)
	{
		slog("Cannot create camera with position NULL"); return NULL;
	}

	if (&size == NULL)
	{
		slog("Cannot create camera with size NULL"); return NULL;
	}

	if (&movementspeed == NULL)
	{
		slog("Cannot create camera with movement speed NULL"); return NULL;
	}

	vector2d_copy(game_cam.pos, pos);
	vector2d_copy(game_cam.size, size);
	vector2d_copy(game_cam.movementspeed, movementspeed);

	game_cam.cam_collider = box_new();
	vector2d_copy(game_cam.cam_collider->position, pos);
	vector2d_copy(game_cam.cam_collider->size, size);

	atexit(camera_free);
}

Bool camera_inview(Collider_Circle* circle, Collider_Box* box)
{
	Bool created_new_box = 0;
	Collider_Box* other_box;

	if (&game_cam == NULL)
	{
		slog("Camera has not been created yet"); return 0;
	}

	if ((circle == NULL) && (box == NULL))
	{
		slog("No valid collider given to check against camera"); return 0;
	}

	//If the input collider is a circle, create a temporary box collider to represent it
	if (circle != NULL)
	{
		created_new_box = 1;

		other_box = box_new();

		vector2d_copy(
			other_box->position, 
			vector2d(
				circle->position.x - circle->radius, 
				circle->position.y - circle->radius
			)
		);

		vector2d_copy(
			other_box->size,
			vector2d(
				circle->position.x + circle->radius,
				circle->position.y + circle->radius
			)
		);
	}

	//Else, set the temp box collider to the input box collider.
	else { other_box = box; }

	return box_detectCollision(game_cam.cam_collider, other_box);

}

void camera_move(Vector2D movement_dir)
{
	Vector2D temp;
	Vector2D movement;

	if ((movement_dir.x != 0) || (movement_dir.y != 0)) { slog("%f, %f", movement_dir.x, movement_dir.y); }

	vector2d_copy(movement, game_cam.movementspeed);

	movement.x = movement.x * movement_dir.x;
	movement.y = movement.y * movement_dir.y;

	vector2d_add(
		temp,
		game_cam.pos, 
		movement
	);

	vector2d_copy(game_cam.pos, temp);
}

void camera_free()
{
	box_free(game_cam.cam_collider);
}