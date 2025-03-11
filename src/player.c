#include "simple_logger.h"
#include "gfc_input.h"

#include "projectile.h"
#include "player.h"

static Entity* thePlayer = NULL;

void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);

Entity* player_get_the()
{
	return thePlayer;
}

typedef struct
{
	int xp, neededxp;
	int inventory[10];
	int lastAttack;
	int cooldown;
}PlayerEntityData;

Entity* player_new()
{
	Entity* self;
	PlayerEntityData* data;
	if (thePlayer)
	{
		//gfc_vector2d_copy(self->position, position);
		return thePlayer;
	}
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new player entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(500,450);

	self->think = player_think;
	self->update = player_update;
	self->free = player_free;
	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	if (data)
	{
		data->neededxp = 1000;
		data->cooldown = 200;
		//data->lastAttack = 0;
	}
	self->data = data;
	thePlayer = self; //
	return self;
}

void player_attack(Entity* self)
{
	if (!self) return;

	PlayerEntityData* data = (PlayerEntityData*)self->data;
	Uint32 curr = SDL_GetTicks();

	if (curr - data->lastAttack < data->cooldown)
	{
		return;
	}

	data->lastAttack = curr;
	projectile_new(self->position);
}

void player_think(Entity* self)
{
	if (!self) return;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	if (keys[SDL_SCANCODE_W]) {
		self->position.y -= 5;
	}
	else if (keys[SDL_SCANCODE_A]) {
		self->position.x -= 5;
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->position.y += 5;
	}
	else if (keys[SDL_SCANCODE_D]) {
		self->position.x += 5;
	}
	if (self->data)
	{
		self->data = (PlayerEntityData*)self->data;
		//do stuff with data
	}

	if (keys[SDL_SCANCODE_SPACE]) {
		player_attack(self);
	}

	/*if (gfc_input_command_down("d")) {
		self->velocity.x = 1.0;
		slog("right");
	}
	else if (gfc_input_command_down("a")) {
		self->velocity.x = -1.0;
	}
	else {
		self->velocity.x = 0;
	}

	if (gfc_input_command_down("s")) {
		self->velocity.y = 1.0;
	}
	else if (gfc_input_command_down("w")) {
		self->velocity.y = -1.0;
	}
	else {
		self->velocity.y = 0;
	}*/
	gfc_vector2d_add(self->position, self->position, self->velocity);
	if (self->position.x < 0) self->position.x = 0;
	if (self->position.y < 0) self->position.y = 0;
}

void player_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void player_free(Entity* self)
{
	PlayerEntityData* data;
	if (!self || !self->data) return;
	data = self->data;
	//other cleanup
	free(data);
	self->data = NULL;
}