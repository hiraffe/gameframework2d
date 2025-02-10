#include "simple_logger.h"

#include "player.h"

void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);


Entity* player_new_entity(Entity* self)
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}
	//gfc_vector2d_copy(self->position, position);
	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0);
	self->frame = 0;
	self->position = vector2d(0, 0);

	self->think = player_think;
	self->update = player_update;
	self->free = player_free;
	return self;
}


void player_think(Entity* self)
{
	if (!self) return;
	if(gfc_input_command_down("right")) {
		self->velocity.x = 5.0;
	}
	else if (gfc_input_command_down("left")) {
		self->velocity.x = -5.0;
	}
	else {
		self->velocity.x = 0;
	}

	if (gfc_input_command_down("down")) {
		self->velocity.y = 5.0;
	}
	else if (gfc_input_command_down("up")) {
		self->velocity.y = -5.0;
	}
	else {
		self->velocity.y = 0;
	}
	gfc_vector2d_add(self->position, self->position, self->velocity);
	if (self->position.x < 0) self->position.x = 0;
	if (self->position.y < 0) self->position.y = 0;
}

void player_update(Entity* self)
{
	if (!self) return;
}

void player_free(Entity* self)
{
	if (!self) return;
}