#include "simple_logger.h"

#include "monster.h"
#include "player.h"

void monster_think(Entity* self);
void monster_update(Entity* self);
void monster_free(Entity* self);

void monster_tester()
{
	Entity* monster1, * monster2, * monster3, * monster4, * monster5;
	monster1 = monster_new(MT_yellow); //add monsters
	monster2 = monster_new(MT_blue);
	monster3 = monster_new(MT_orange);
	monster4 = monster_new(MT_red);
	monster5 = monster_new(MT_green);
}

Entity* monster_new(MonsterType type)
{
	Entity* self;
	MonsterEntityData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new monster entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/monster.png",
		44,
		44,
		3,
		0);
	self->frame = 0;
	self->position = gfc_vector2d(0, 0);
	self->position = gfc_vector2d(0, 0);
	self->team = ETT_monster;
	self->bounds = (GFC_Rect){ self->position.x + 12,self->position.y + 12,20,20 };

	self->think = monster_think;
	self->update = monster_update;
	self->free = monster_free;

	data = gfc_allocate_array(sizeof(MonsterEntityData), 1);
	if (data)
	{
		data->type = type;
		switch (type)
		{
		case MT_red:
			self->position = gfc_vector2d(600, 300);
			break;
		case MT_green:
			self->position = gfc_vector2d(600, 400);
			break;
		case MT_blue:
			self->position = gfc_vector2d(0, (rand() % 650));
			self->velocity = gfc_vector2d(-1, 0);
			break;
		case MT_orange:
			self->position = gfc_vector2d((rand() % 1000), 0);
			break;
		case MT_yellow:
			self->position = gfc_vector2d(0, 100);
			break;
		default:
			self->position = gfc_vector2d((rand() % 1000) + 1, 0);
		}
	}
	self->data = data;

	return self;
}


void monster_think(Entity* self)
{
	if (!self) return;
	MonsterEntityData* data = (MonsterEntityData*)self->data;
	
	GFC_Vector2D dir = { 0 };

	if (data->type == MT_yellow)
	{		
		//moves in a wave
		// Sinusoidal vertical movement
		float amplitude = 10.0f; // Amplitude of the wave
		float frequency = 0.05f; // Frequency of the wave
		dir.y = sin(self->position.x * frequency) * amplitude;
		dir.x = 1; // Constant horizontal movement to the right
	}
	else if (data->type == MT_blue)
	{
		//bounces back and forth
		// Check for collision with screen boundaries
		if (self->position.x <= 0 || self->position.x >= 1200 - self->sprite->frame_w)
		{
			self->velocity.x = -self->velocity.x;
		}
		dir.x = self->velocity.x;
	}
	else if (data->type == MT_orange)
	{
		//goes straight down
		dir.y = 1;
	}
	else if (data->type == MT_green)
	{
		//follows the mouse
		Sint32 mx = 0, my = 0;
		SDL_GetMouseState(&mx, &my);
		if (self->position.x < mx) dir.x = 1;
		if (self->position.y < my) dir.y = 1;
		if (self->position.x > mx) dir.x = -1;
		if (self->position.y > my) dir.y = -1;
	}
	else if (data->type == MT_red)
	{
		//follows the player
		Sint32 px = 0, py = 0;
		Entity* player = player_get_the();
		if (!player) return;
		px = player->position.x;
		py = player->position.y;
		if (self->position.x < px) dir.x = 1;
		if (self->position.y < py) dir.y = 1;
		if (self->position.x > px) dir.x = -1;
		if (self->position.y > py) dir.y = -1;
	}
	gfc_vector2d_normalize(&dir);
	gfc_vector2d_scale(self->velocity, dir, 3);
}

void monster_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 8) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);

	self->bounds = (GFC_Rect){ self->position.x + 12,self->position.y + 12,20,20 };
}

void monster_free(Entity* self)
{
	MonsterEntityData* data;
	if (!self || !self->data) return;
	data = self->data;
	free(data);
	self->data = NULL;
	entity_free(self);
}