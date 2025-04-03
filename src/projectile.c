#include "simple_logger.h"

#include "projectile.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
void projectile_free(Entity* self);

Entity* projectile_new(GFC_Vector2D start, ProjectileDir dir)
{
	Entity* self;
	ProjectileData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new projectile entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/boolet.png",
		36,
		12,
		1,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(start.x + 12, start.y + 12);

	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;

	data = gfc_allocate_array(sizeof(ProjectileData), 1);
	if (data)
	{
		data->direction = dir;
	}
	self->data = data;
	return self;
}

void spawn_projectile(GFC_Vector2D start, ProjectileDir dir, ProjectileNum num) 
{
	//if double, double, if triple, triple
	if (num == PN_double)
	{
		Entity* projectile1 = projectile_new(gfc_vector2d(start.x+30,start.y), dir);
		Entity* projectile2 = projectile_new(gfc_vector2d(start.x-30, start.y), dir);
		if (!projectile1 || !projectile2) return;
	}
	else if (num == PN_triple)
	{
		Entity* projectile1 = projectile_new(gfc_vector2d(start.x, start.y), dir);
		Entity* projectile2 = projectile_new(gfc_vector2d(start.x-50, start.y), dir);
		Entity* projectile3 = projectile_new(gfc_vector2d(start.x+50, start.y), dir);
		if (!projectile1 || !projectile2 || !projectile3) return;
	}
	else if (num == PN_all)
	{
		Entity* projectile1 = projectile_new(gfc_vector2d(start.x, start.y), PD_up);
		Entity* projectile2 = projectile_new(gfc_vector2d(start.x, start.y), PD_down);
		Entity* projectile3 = projectile_new(gfc_vector2d(start.x, start.y), PD_left);
		Entity* projectile4 = projectile_new(gfc_vector2d(start.x, start.y), PD_right);
		if (!projectile1 || !projectile2 || !projectile3 || !projectile4) return;
	}
	else
	{
		Entity* projectile = projectile_new(start, dir);
		if (!projectile) return;
	}
}

//void projectile_hit(Entity *self)
//free if it hits something

void projectile_think(Entity* self)
{
	if (!self) return;

	ProjectileData* data = (ProjectileData*)self->data;
	switch (data->direction)
	{
		case PD_up:
			self->velocity.y = -5.0;
			break;
		case PD_down:
			self->velocity.y = 5.0;
			break;
		case PD_left:
			self->velocity.x = -5.0;
			break;
		case PD_right:
			self->velocity.x = 5.0;
			break;
		default:
			self->velocity.y = -5.0;
	}

	gfc_vector2d_add(self->position, self->position, self->velocity);
	//if (self->position.x < 0) self->position.x = 0;
	//if (self->position.y < 0) self->position.y = 0;

	//if projectile hits a wall or enemy, it dies
	//code to check what type of thing it hits?
}

void projectile_update(Entity* self)
{
	if (!self) return;
	/*
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;
	*/

	gfc_vector2d_add(self->position, self->position, self->velocity);

	// Define screen boundaries
	int screen_width = 1200;
	int screen_height = 720;

	// Check if projectile is out of screen bounds
	if (self->position.x < 0 || self->position.x > screen_width ||
		self->position.y < 0 || self->position.y > screen_height)
	{
		// Free the projectile entity
		projectile_free(self);
	}
		
}

void projectile_free(Entity* self)
{
	ProjectileData* data;
	data = self->data;
	if (!self || !self->data) return;
	//other cleanup
	free(data);
	self->data = NULL;
	entity_free(self);
}