#include "simple_logger.h"

#include "enemy.h"
#include "monster.h"
#include "spawner.h"

void enemy_think(Entity* self);
void enemy_update(Entity* self);
void enemy_free(Entity* self);
void enemy_on_hit(Entity* self, int dmg);

static SJson* _enemyJson = NULL;
static SJson* _enemyDefs = NULL;

void monster_tester()
{
	Entity* monster1, * monster2, * monster3, * monster4, * monster5;
	//monster1 = monster_new(MT_wave); //add monsters
	//monster2 = monster_new(MT_hbounce);
	//monster3 = monster_new(MT_down);
	//monster4 = monster_new(MT_pfollow);
	//monster5 = monster_new(MT_mfollow);
}

void enemies_close()
{
	if (_enemyJson)
	{
		sj_free(_enemyJson);
	}
	_enemyJson = NULL;
	_enemyDefs = NULL;
}

void enemies_init(const char* filename)
{
	if (!filename)
	{
		slog("no filename provided for enemy initialization");
		return;
	}
	_enemyJson = sj_load(filename);
	if (!_enemyJson)
	{
		slog("failed to load the json for the enemy definition");
		return;
	}
	_enemyDefs = sj_object_get_value(_enemyJson, "enemies");
	if (!_enemyDefs)
	{
		slog("enemy definition file %s does not contain enemies list", filename);
		sj_free(_enemyJson);
		_enemyJson = NULL;
		return;
	}
	atexit(enemies_close);
}

SJson* enemies_get_def_by_name(const char* name)
{
	int i, c;
	SJson* enemy;
	const char* enemyName = NULL;
	if (!name) return NULL;
	if (!_enemyDefs)
	{
		slog("no enemy definitions loaded");
		return NULL;
	}
	c = sj_array_get_count(_enemyDefs);
	for (int i = 0; i < c; i++)
	{
		enemy = sj_array_get_nth(_enemyDefs, i);
		if (!enemy) continue;
		enemyName = sj_object_get_value_as_string(enemy, "name");
		if (!enemyName) continue;
		if (gfc_strlcmp(name, enemyName) == 0) {
			//found it
			return enemy;
		}
	}
	slog("no enemy found by name", name);
	return NULL;
}

/*
Entity* enemy_new_test()
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
		"images/moth_girl.png",
		128,
		384,
		2,
		0);
	self->frame = 1;
	self->position = gfc_vector2d(1300, 150);
	self->team = ETT_monster;
	self->health = 100;
	self->bounds = (GFC_Rect){ self->position.x,self->position.y,self->sprite->frame_w, self->sprite->frame_h };

	self->onHit = enemy_on_hit;
	self->think = enemy_think;
	self->update = enemy_update;
	self->free = enemy_free;

	data = gfc_allocate_array(sizeof(EnemyEntityData), 1);
	if (data)
	{
		data->monster_max = 10;
	}
	self->data = data;
	return self;
}
*/

Entity* enemy_new(const char* name)
{
	Entity* self;
	EnemyEntityData* data;
	SJson* def;
	const char* sprite_img;
	int frame_w, frame_h, y, health;

	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new enemy entity");
		return NULL;
	}

	def = enemies_get_def_by_name(name);

	sprite_img = sj_object_get_value_as_string(def, "image");
	sj_object_get_value_as_int(def, "frame_w", &frame_w);
	sj_object_get_value_as_int(def, "frame_h", &frame_h);
	self->sprite = gf2d_sprite_load_all(
		sprite_img,
		frame_w,
		frame_h,
		2,
		0);
	self->frame = 1;
	sj_object_get_value_as_int(def, "y", &y);
	self->position = gfc_vector2d(1300, y);
	self->team = ETT_monster;
	sj_object_get_value_as_int(def, "health", &health);
	self->health = health;
	self->bounds = (GFC_Rect){ self->position.x,self->position.y,self->sprite->frame_w, self->sprite->frame_h };

	self->onHit = enemy_on_hit;
	self->think = enemy_think;
	self->update = enemy_update;
	self->free = enemy_free;

	data = gfc_allocate_array(sizeof(EnemyEntityData), 1);
	if (data)
	{
		data->monster_max = 10;
	}
	self->data = data;
	return self;
}

void enemy_on_hit(Entity* self, int dmg)
{
	self->health -= dmg;
	self->frame = 0;
	if (self->health < 0) {
		Spawner* spawner = spawner_get_the();
		spawner->alive--;
		enemy_free(self);
		slog("enemy killed");
	}
}

void enemy_think(Entity* self)
{
	if (!self) return;
	EnemyEntityData* data = (EnemyEntityData*)self->data;

	while (data->monster_max > 0)
	{
		monster_tester();
		data->monster_max--;
	}

	if (self->position.x >= 1000)
	{
		self->position.x -= 4;
	}
	
	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void enemy_update(Entity* self)
{
	if (!self) return;

	//self->frame += 0.1;
	//if (self->frame >= 16) self->frame = 0;
	if (self->frame < 1)
	{
		self->frame += 0.1;
	}
	if (self->frame > 1) self->frame = 1;

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
