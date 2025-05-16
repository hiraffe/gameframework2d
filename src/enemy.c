#include "simple_logger.h"
#include "gfc_audio.h"

#include "enemy.h"

void enemy_think(Entity* self);
void enemy_update(Entity* self);
void enemy_free(Entity* self);
void enemy_on_hit(Entity* self, int dmg);

static SJson* _enemyJson = NULL;
static SJson* _enemyDefs = NULL;

void monster_tester()
{
	//Entity* monster1, * monster2, * monster3, * monster4, * monster5;
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

Entity* enemy_new(const char* id, const char* name)
{
	Entity* self;
	EnemyEntityData* data;
	SJson* def;
	const char* sprite_img;
	int frame_w, frame_h, y, monsters, delay, boss;
	float health;

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
	self->position = gfc_vector2d(1200, y); //non boss go further up
	self->team = ETT_monster;
	sj_object_get_value_as_float(def, "health", &health);
	self->health = health;
	self->bounds = (GFC_Rect){ self->position.x,self->position.y,self->sprite->frame_w, self->sprite->frame_h };

	self->onHit = enemy_on_hit;
	self->think = enemy_think;
	self->update = enemy_update;
	self->free = enemy_free;

	data = gfc_allocate_array(sizeof(EnemyEntityData), 1);
	if (data)
	{
		data->name = name;
		data->id = id;
		data->monster_count = 0;
		sj_object_get_value_as_int(def, "monsters", &monsters);
		data->monster_max = monsters;
		data->delaying = 0;
		sj_object_get_value_as_int(def, "delay", &delay);
		data->delay = delay;
		data->health_max = health;
		sj_object_get_value_as_int(def, "boss", &boss);
		data->boss = boss;
		if (boss == 1)
		{
			self->position = gfc_vector2d(1400, y);	//boss stays behind
			const char* enemy1, enemy2;
			data->boss_data = gfc_allocate_array(sizeof(EnemyBossData), 1);
			data->boss_data->enemy1 = sj_object_get_value_as_string(def, "first_enemy");
			data->boss_data->enemy2 = sj_object_get_value_as_string(def, "second_enemy");
			data->boss_data->enemy1_spawned = 0;
			data->boss_data->enemy2_spawned = 0;
		}
	}
	self->data = data;
	return self;
}

void enemy_clear_all_monsters(Entity* self)
{
	EnemyEntityData* data = (EnemyEntityData*)self->data;
	if (!self || !data) return;

	//kill all monsters that were spawned by this guy
	EntitySystem entity_system = entity_get_system();
	for (int i = 0; i < entity_system.entity_max; i++)
	{
		Entity* child = &entity_system.entity_list[i];
		if (!child || child->team != ETT_monster) continue;
		MonsterEntityData* child_data = (MonsterEntityData*)child->data;
		if (!child_data) continue;
		//slog("child: %s, parent: %s", child_data->parent_id, data->id);
		if (strcmp(child_data->parent_id, data->id) == 0)
		{
			monster_free(child);
		}
	}
}

void enemy_on_hit(Entity* self, int dmg)
{
	EnemyEntityData* data = (EnemyEntityData*)self->data;
	if (!self || !data) return;

	self->health -= dmg;
	self->frame = 0;

	if (fmodf(self->health, 20.0f) == 0.0f)
	{
		Mix_Chunk* sound = Mix_LoadWAV("audio/hit.wav");
		int channel = Mix_PlayChannel(-1, sound, 0);
	}

	//death
	if (self->health < 0) {
		Mix_Chunk* sound = Mix_LoadWAV("audio/vine-boom.wav");
		int channel = Mix_PlayChannel(-1, sound, 0);

		if (data->boss == 1)
		{
			//free the friends
			EnemyBossData* boss_data = (EnemyBossData*)data->boss_data;
			if (!boss_data) return;
			enemy_clear_all_monsters(boss_data->friend1);
			enemy_free(boss_data->friend1); 
			enemy_clear_all_monsters(boss_data->friend2);
			enemy_free(boss_data->friend2); 
		}

		Spawner* spawner = spawner_get_the();
		spawner->alive--;
		spawner->dead++;

		enemy_clear_all_monsters(self);
		enemy_free(self);
		slog("enemy killed");
	}
}

void enemy_spawn_monsters (Entity* self, const char* name)
{
	if (!self) return;
	EnemyEntityData* data = (EnemyEntityData*)self->data;
	Entity* monster;

	if (strcmp(name, "bug") == 0)
	{
		monster = monster_new(MT_hbounce, data->id);
	}
	else if (strcmp(name, "guy") == 0)
	{
		monster = monster_new(MT_down, data->id);
	}
	else if (strcmp(name, "ghost") == 0)
	{
		monster = monster_new(MT_smallwave, data->id);
	}
	else if (strcmp(name, "googly") == 0)
	{
		monster = monster_new(MT_largewave, data->id);
	}
	else if (strcmp(name, "girl") == 0)
	{
		monster = monster_new(MT_pfollow, data->id);
	}

	data->monster_count++;
}

void enemy_think(Entity* self)
{
	if (!self) return;
	EnemyEntityData* data = (EnemyEntityData*)self->data;
	Uint32 now = SDL_GetTicks();

	//move into position
	if (data->boss == 1 && self->position.x >= 1100)
	{
		self->position.x -= 4;
	}
	else if (self->position.x >= 900)
	{
		self->position.x -= 4;
	}

	//check if we're in a waiting state
	if (data->delaying)
	{
		if (now - data->spawn_time >= data->delay) // 3000 ms = 3 seconds
		{
			data->monster_count = 0;
			data->delaying = 0;
		}
		return;
	}
	
	//while (data->monster_max > 0)
	while(data->monster_count <= data->monster_max)
	{
		enemy_spawn_monsters(self, data->name);
		data->monster_count++;
	}

	if (strcmp(data->name, "bug") != 0 && data->monster_count >= data->monster_max)
	{
		data->spawn_time = SDL_GetTicks();  // Start the delay timer
		data->delaying = 1;
		data->monster_count = 0;
	}

	if (data->boss == 1)
	{
		EnemyBossData* boss_data = (EnemyBossData*)data->boss_data;
		if (!boss_data) return;

		if (self->health < 2 * (data->health_max / 3) && boss_data->enemy1_spawned == 0)
		{
			slog("enemy1: %s", boss_data->enemy1);
			boss_data->friend1 = enemy_new("boss_friend_1", boss_data->enemy1);
			boss_data->enemy1_spawned = 1;
		}
		if (self->health < data->health_max/3 && boss_data->enemy2_spawned == 0)
		{
			boss_data->friend2 = enemy_new("boss_friend_2", boss_data->enemy2);
			boss_data->enemy2_spawned = 1;
		}
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
	if (data->boss = 1)
	{
		EnemyBossData* boss_data;
		boss_data = data->boss_data;
		free(boss_data);
		data->boss_data = NULL;
	}
	free(data);
	self->data = NULL;
	entity_free(self);
}
