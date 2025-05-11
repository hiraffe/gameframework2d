#include "simple_logger.h"
#include "simple_json.h"
#include "gfc_input.h"

#include "gf2d_draw.h"

#include "player.h"

static Entity* thePlayer = NULL;

void player_think(Entity* self);
void player_update(Entity* self);
void player_free(Entity* self);
void player_on_hit(Entity* self, int dmg);

static SJson* _classJson = NULL;
static SJson* _classDefs = NULL;

Entity* player_get_the()
{
	return thePlayer;
}

void player_classes_close()
{
	if (_classJson)
	{
		sj_free(_classJson);
	}
	_classJson = NULL;
	_classDefs = NULL;
}

void player_classes_init(const char* filename)
{
	if (!filename)
	{
		slog("no filename provided for class initialization");
		return;
	}
	_classJson = sj_load(filename);
	if (!_classJson)
	{
		slog("failed to load the json for the class definition");
		return;
	}
	_classDefs = sj_object_get_value(_classJson, "classes");
	if (!_classDefs)
	{
		slog("item definition file %s does not contain items list", filename);
		sj_free(_classJson);
		_classJson = NULL;
		return;
	}
	atexit(player_classes_close);
}

SJson* player_classes_get_def_by_name(const char* name)
{
	int i, c;
	SJson* class;
	const char* className = NULL;
	if (!name) return NULL;
	if (!_classDefs)
	{
		slog("no class definitions loaded");
		return NULL;
	}
	c = sj_array_get_count(_classDefs);
	for (int i = 0; i < c; i++)
	{
		class = sj_array_get_nth(_classDefs, i);
		if (!class) continue;
		className = sj_object_get_value_as_string(class, "name");
		if (!className) continue;
		if (gfc_strlcmp(name, className) == 0) {
			//found it
			return class;
		}
	}
	slog("no class found by name", name);
	return NULL;
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
	self->position = gfc_vector2d(500, 450);
	self->team = ETT_player;
	self->health = 100;
	self->bounds = (GFC_Rect){ self->position.x + 8,self->position.y + 8,28,28 };

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
		data->tp = 0;
	}
	self->data = data;
	thePlayer = self; //
	return self;
}
/*
Entity* player_new(const char* type)
{
	Entity* self;
	PlayerEntityData* data;
	SJson* def;
	const char* sprite_img;
	int frame_w, frame_h, fpl, health, speed, cooldown;

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
	def = player_classes_get_def_by_name(type);

	sprite_img = sj_object_get_value_as_string(def, "image");
	sj_object_get_value_as_int(def, "frame_w", &frame_w);
	sj_object_get_value_as_int(def, "frame_h", &frame_h);
	sj_object_get_value_as_int(def, "fpl", &fpl);
	self->sprite = gf2d_sprite_load_all(
		sprite_img,
		frame_w,
		frame_h,
		fpl,
		0);
	
	self->frame = 0;
	self->position = gfc_vector2d(500,450);
	self->team = ETT_player;
	sj_object_get_value_as_int(def, "health", &health);
	self->health = health;
	self->bounds = (GFC_Rect){self->position.x+8,self->position.y+8,28,28};

	self->onHit = player_on_hit;
	self->think = player_think;
	self->update = player_update;
	self->free = player_free;

	data = gfc_allocate_array(sizeof(PlayerEntityData), 1);
	if (data)
	{
		data->neededxp = 1000;
		data->special = sj_object_get_value_as_string(def, "special");
		sj_object_get_value_as_int(def, "cooldown", &cooldown);
		data->cooldown = cooldown;
		data->power = PU_none;
		sj_object_get_value_as_int(def, "speed", &speed);
		data->speed = speed;
		data->nearmiss = (GFC_Rect){ self->position.x, self->position.y, self->sprite->frame_w, self->sprite->frame_h };
		data->tp = 0;
	}
	self->data = data;
	thePlayer = self; //
	return self;
}
*/
void player_change_class(const char* type)
{
	Entity* self = player_get_the();
	PlayerEntityData* data = (PlayerEntityData*)self->data;
	if (!self || !self->data) return;
	SJson* def;
	const char* sprite_img;
	int frame_w, frame_h, fpl, health, speed, cooldown;

	def = player_classes_get_def_by_name(type);

	sprite_img = sj_object_get_value_as_string(def, "image");
	sj_object_get_value_as_int(def, "frame_w", &frame_w);
	sj_object_get_value_as_int(def, "frame_h", &frame_h);
	sj_object_get_value_as_int(def, "fpl", &fpl);
	self->sprite = gf2d_sprite_load_all(
		sprite_img,
		frame_w,
		frame_h,
		fpl,
		0);

	sj_object_get_value_as_int(def, "health", &health);
	self->health = health;

	data->neededxp = 1000;

	data->special = sj_object_get_value_as_string(def, "special");

	sj_object_get_value_as_int(def, "cooldown", &cooldown);
	data->cooldown = cooldown;

	data->power = PU_none;

	sj_object_get_value_as_int(def, "speed", &speed);
	data->speed = speed;

	slog("class changed to %s", type);
}

void player_attack(Entity* self, ProjectileDir dir)
{
	if (!self) return;

	PlayerEntityData* data = (PlayerEntityData*)self->data;
	Uint32 curr = SDL_GetTicks();

	if (data->power == PU_reload) {
		data->cooldown = 200;
	} else { 
		data->cooldown = 400;
	}

	if (curr - data->lastAttack < data->cooldown) {
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

void player_on_hit(Entity* self, int dmg)
{
	self->health -= dmg;
	slog("health: %f", self->health);
	if (self->health <= 0) {
		slog("You Died!");
	}
}

void player_think(Entity* self)
{
	if (!self) return;
	PlayerEntityData* data = (PlayerEntityData*)self->data;
	EntitySystem entity_system = entity_get_system();
	GFC_Vector2D move;

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

	//move = self->position;
	move = (GFC_Vector2D){ 0,0 };
	if (keys[SDL_SCANCODE_W]) {
		move.y -= data->speed;
	}
	else if (keys[SDL_SCANCODE_A]) {
		move.x -= data->speed;
	}
	else if (keys[SDL_SCANCODE_S]) {
		move.y += data->speed;
	}
	else if (keys[SDL_SCANCODE_D]) {
		move.x += data->speed;
	}
	entity_move(self, move);

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

	//gfc_vector2d_add(self->position, self->position, self->velocity);
	//if (self->position.x < 0) self->position.x = 0;
	//if (self->position.y < 0) self->position.y = 0;

	//check for collision with an enemy;
	for (int i = 0; i < entity_system.entity_max; i++)
	{
		Entity* other = &entity_system.entity_list[i];
		if (!other->_inuse || other == self) continue;

		if (other->team == ETT_monster)
		{
			if (entity_collision(self->bounds, other->bounds))
			{
				if (other->onHit)
				{
					other->onHit(other, 1);  // apply 10 damage
				}
				if (self->onHit)
				{
					self->onHit(self, other->dmg);
				}
				break;
			} 
			else if (entity_collision(data->nearmiss, other->bounds))
			{
				data->tp++;
				//slog("tp: %d", data->tp);
			}
		}
	}
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