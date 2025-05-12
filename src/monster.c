#include "simple_logger.h"

#include "monster.h"
#include "player.h"

void monster_think(Entity* self);
void monster_update(Entity* self);
void monster_free(Entity* self);
void monster_on_hit(Entity* self, int dmg);

Entity* monster_new(MonsterType type, const char* parent_id)
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
	//self->position = gfc_vector2d(0, 0);
	self->team = ETT_monster;
	self->health = 1;
	self->dmg = 1;
	self->bounds = (GFC_Rect){ self->position.x + 12,self->position.y + 12,20,20 };

	self->onHit = monster_on_hit;
	self->think = monster_think;
	self->update = monster_update;
	self->free = monster_free;

	data = gfc_allocate_array(sizeof(MonsterEntityData), 1);
	if (data)
	{
		data->parent_id = parent_id;
		//slog("parent_id: %s", data->parent_id);
		data->type = type;
		switch (type)
		{
		case MT_pfollow:
			self->position = gfc_vector2d(600, 300);
			break;
		case MT_mfollow:
			self->position = gfc_vector2d(600, 400);
			break;
		case MT_hbounce:
			self->position = gfc_vector2d(100, (rand() % (600 - 50))+50);
			self->velocity = gfc_vector2d(-1, 0);
			break;
		case MT_down:
			self->position = gfc_vector2d((rand() % 1000), 50);
			break;
		case MT_wave:
			self->position = gfc_vector2d(0, 100);
			break;
		default:
			self->position = gfc_vector2d((rand() % 1000) + 1, 0);
		}
	}
	self->data = data;
	return self;
}

void monster_on_hit(Entity *self, int dmg)
{
	self->health -= dmg;
	if (self->health < 0) {
		monster_free(self);
	}
}

void monster_think(Entity* self)
{
	if (!self) return;
	MonsterEntityData* data = (MonsterEntityData*)self->data;
	
	GFC_Vector2D dir = { 0 };
	GFC_Vector2D move;
	float speed = 2.0f;

	if (data->type == MT_wave)
	{		
		//moves in a sin wave
		float amplitude = 10.0f; 
		float frequency = 0.05f; 
		dir.y = sin(self->position.x * frequency) * amplitude;
		dir.x = 3; // Constant horizontal movement to the right
	}
	else if (data->type == MT_hbounce)
	{
		//bounces back and forth
		// Check for collision with screen boundaries
		if (self->position.x <= 32 || self->position.x >= 860 - self->sprite->frame_w)
		{
			self->velocity.x = -self->velocity.x;
		}
		dir.x = self->velocity.x;
	}
	else if (data->type == MT_down)
	{
		//goes straight down
		dir.y = 1;
	}
	else if (data->type == MT_mfollow)
	{
		//follows the mouse
		Sint32 mx = 0, my = 0;
		SDL_GetMouseState(&mx, &my);
		if (self->position.x < mx) dir.x = 1;
		if (self->position.y < my) dir.y = 1;
		if (self->position.x > mx) dir.x = -1;
		if (self->position.y > my) dir.y = -1;
	}
	else if (data->type == MT_pfollow)
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
	gfc_vector2d_scale(self->velocity, dir, speed);

	GFC_Vector2D newPos;
	gfc_vector2d_copy(newPos, self->position);
	gfc_vector2d_add(newPos, newPos, self->velocity);

	if (!entity_move(self, self->velocity))
	{
		slog("cant move, position: %f, %f", self->position.x, self->position.y);
		monster_free(self);
	}
	else
	{
		//slog("yes move, position: %f, %f", self->position.x, self->position.y);
	}
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