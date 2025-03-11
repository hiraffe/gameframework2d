#include "simple_logger.h"

#include "monster.h"
#include "player.h"

void monster_think(Entity* self);
void monster_update(Entity* self);
void monster_free(Entity* self);

Entity* monster_new()
{
	Entity* self;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new monster entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/ed210.png",
		128,
		128,
		16,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(100, 100);

	self->think = monster_think;
	self->update = monster_update;
	self->free = monster_free;
	return self;
}


void monster_think(Entity* self)
{
	//have it change for each different guy idk
	if (!self) return;
	
	GFC_Vector2D dir = { 0 };
	Sint32 mx = 0, my = 0;
	Entity* player = player_get_the();
	if (!self || !player) return;
	//SDL_GetMouseState(&mx, &my);
	mx = player->position.x;
	my = player->position.y;
	if (self->position.x < mx) dir.x = 1;
	if (self->position.y < my) dir.y = 1;
	if (self->position.x > mx) dir.x = -1;
	if (self->position.y > my) dir.y = -1;
	gfc_vector2d_normalize(&dir);
	gfc_vector2d_scale(self->velocity, dir, 3);
}

void monster_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void monster_free(Entity* self)
{
	if (!self) return;
	entity_free(self);
}