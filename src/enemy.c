#include "simple_logger.h"

#include "enemy.h"

void enemy_think(Entity* self);
void enemy_update(Entity* self);
void enemy_free(Entity* self);

Entity* enemy_new()
{
	Entity* self;
	EnemyEntityData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new enemy entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(1300, 350);
	self->team = ETT_monster;
	self->bounds = (GFC_Rect){ self->position.x,self->position.y,self->sprite->frame_w, self->sprite->frame_h };

	self->think = enemy_think;
	self->update = enemy_update;
	self->free = enemy_free;

	data = gfc_allocate_array(sizeof(EnemyEntityData), 1);
	if (data)
	{
		data->smth = 0;
	}
	self->data = data;
	return self;
}

void enemy_think(Entity* self)
{
	if (!self) return;
	EnemyEntityData* data = (EnemyEntityData*)self->data;

	if (self->position.x >= 1000)
	{
		self->position.x -= 4;
	}
	
	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void enemy_update(Entity* self)
{
	if (!self) return;

	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);

	self->bounds = (GFC_Rect){ self->position.x,self->position.y,self->sprite->frame_w, self->sprite->frame_h };
}

void enemy_free(Entity* self)
{
	EnemyEntityData* data;
	if (!self || !self->data) return;
	data = self->data;
	free(data);
	self->data = NULL;
	entity_free(self);
}
