#include "simple_logger.h"

#include "item.h"
#include "player.h"

void item_think(Entity* self);
void item_update(Entity* self);
void item_free(Entity* self);

typedef struct
{
	PowerUp power;
	int powertime;
}ItemEntityData;

Entity* item_new(PowerUp powerup)
{
	Entity* self;
	ItemEntityData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new item entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/pointer.png",
		32,
		32,
		16,
		0);
	self->frame = 0;
	//self->position = gfc_vector2d(0, 0);
	switch (powerup) //just for displaying
	{
		case PU_double:
			self->position = gfc_vector2d(50, 650);
			break;
		case PU_triple:
			self->position = gfc_vector2d(250, 650);
			break;
		case PU_quad:
			self->position = gfc_vector2d(450, 650);
			break;
		case PU_speedy:
			self->position = gfc_vector2d(650, 650);
			break;
		case PU_reload:
			self->position = gfc_vector2d(850, 650);
			break;
	}

	self->think = item_think;
	self->update = item_update;
	self->free = item_free;

	data = gfc_allocate_array(sizeof(ItemEntityData), 1);
	if (data)
	{
		data->power = powerup;
		data->powertime = 3000;
	}
	self->data = data;

	return self;
}

void give_powerup(Entity* self, Entity* player)
{
	if (!self || !player) return;
	ItemEntityData* data = (ItemEntityData*)self->data;
	PlayerEntityData* p_data = (PlayerEntityData*)player->data;
	Uint32 curr = SDL_GetTicks();

	p_data->power = data->power;
	p_data->powerExpiry = curr + data->powertime;
}

void item_think(Entity* self)
{
	if (!self) return;

	//if player touches it, give them ability
	Entity* player = player_get_the();

	Bounds item_bounds = {
		self->position.x,
		self->position.y,
		self->sprite->frame_w,
		self->sprite->frame_h
	};

	Bounds player_bounds = {
		player->position.x,
		player->position.y,
		player->sprite->frame_w,
		player->sprite->frame_h
	};

	// Check for collision
	if (entity_collision(item_bounds, player_bounds)) {
		give_powerup(self, player);
		return; // Exit after handling collision
	}
}

void item_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void item_free(Entity* self)
{
	ItemEntityData* data;
	if (!self || !self->data) return;
	data = self->data;
	//other cleanup
	free(data);
	self->data = NULL;
}