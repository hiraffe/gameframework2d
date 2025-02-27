#include "simple_logger.h"

#include "projectile.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
void projectile_free(Entity* self);

Entity* projectile_new(GFC_Vector2D start)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new projectile entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/space_bug_top.png",
		128,
		128,
		16,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(start.x, start.y);

	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;
	return self;
}


void projectile_think(Entity* self)
{
	if (!self) return;

	self->velocity.y = -5.0;

	gfc_vector2d_add(self->position, self->position, self->velocity);
	if (self->position.x < 0) self->position.x = 0;
	if (self->position.y < 0) self->position.y = 0;
}

void projectile_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void projectile_free(Entity* self)
{
	if (!self) return;
	entity_free(self);
}