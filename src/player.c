#include "simple_logger.h"
#include "gfc_input.h"

#include "gf2d_draw.h"

#include "player.h"

static Entity* thePlayer = NULL;

void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);

Entity* player_get_the()
{
	return thePlayer;
}

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
		"images/player2.png",
		44,
		44,
		3,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(500,450);
	self->team = ETT_player;
	self->health = 50;
	self->bounds = (GFC_Rect){self->position.x+8,self->position.y+8,28,28};

	self->think = player_think;
	self->update = player_update;
	self->free = player_free;

	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	if (data)
	{
		data->neededxp = 1000;
		data->cooldown = 400;
		data->power = PU_none;
		data->speed = 5;
		data->nearmiss = (GFC_Rect){ self->position.x, self->position.y, self->sprite->frame_w, self->sprite->frame_h };
	}
	self->data = data;
	thePlayer = self; //
	return self;
}

void player_attack(Entity* self, ProjectileDir dir)
{
	if (!self) return;

	PlayerEntityData* data = (PlayerEntityData*)self->data;
	Uint32 curr = SDL_GetTicks();

	if (data->power == PU_reload)
	{
		data->cooldown = 200;
	}
	else
	{
		data->cooldown = 400;
	}

	if (curr - data->lastAttack < data->cooldown)
	{
		return;
	}
	data->lastAttack = curr;

	if (data->power == PU_double)
	{
		spawn_projectile(self->position, dir, PN_double);
	}
	else if (data->power == PU_triple)
	{
		spawn_projectile(self->position, dir, PN_triple);
	}
	else if (data->power == PU_quad)
	{
		spawn_projectile(self->position, PD_all, PN_all);
	}
	else
	{
		spawn_projectile(self->position, dir, PN_single);
	}
}

void player_think(Entity* self)
{
	if (!self) return;
	PlayerEntityData* data = (PlayerEntityData*)self->data;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	Uint32 curr = SDL_GetTicks();

	if (data->power != PU_none && curr > data->powerExpiry)
	{
		data->power = PU_none;
	}

	if (data->power == PU_speedy)
	{
		data->speed = 8;
	}
	else
	{
		data->speed = 4;
	}

	if (keys[SDL_SCANCODE_W]) {
		self->position.y -= data->speed;
	}
	else if (keys[SDL_SCANCODE_A]) {
		self->position.x -= data->speed;
	}
	else if (keys[SDL_SCANCODE_S]) {
		self->position.y += data->speed;
	}
	else if (keys[SDL_SCANCODE_D]) {
		self->position.x += data->speed;
	}
	if (self->data)
	{
		self->data = (PlayerEntityData*)self->data;
		//do stuff with data
	}

	if (keys[SDL_SCANCODE_UP]) {
		player_attack(self, PD_up);
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		player_attack(self, PD_down);
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		player_attack(self, PD_left);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		player_attack(self, PD_right);
	}

	gfc_vector2d_add(self->position, self->position, self->velocity);
	if (self->position.x < 0) self->position.x = 0;
	if (self->position.y < 0) self->position.y = 0;
}

void player_update(Entity* self)
{
	if (!self) return;
	PlayerEntityData* data = (PlayerEntityData*)self->data;

	self->frame += 0.1;
	if (self->frame >= 10) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);

	self->bounds = (GFC_Rect){ self->position.x + 12,self->position.y + 12,20,20 };
	data->nearmiss = (GFC_Rect){ self->position.x, self->position.y, self->sprite->frame_w, self->sprite->frame_h };
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