#include "simple_logger.h"

#include "projectile.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
void projectile_free(Entity* self);

Entity* projectile_new(GFC_Vector2D start, ProjectileDir dir)
{
	Entity* self;
	ProjectileData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new projectile entity");
		return NULL;
	}
	if (dir == PD_up || dir == PD_down) {
		self->sprite = gf2d_sprite_load_all(
			"images/boolet2.png",
			12,
			36,
			1,
			0);
	}
	if (dir == PD_right || dir == PD_left) {
		self->sprite = gf2d_sprite_load_all(
			"images/boolet.png",
			36,
			12,
			1,
			0);
	}
	self->frame = 0;
	self->position = gfc_vector2d(start.x + 5, start.y + 5);
	self->team = ETT_player;
	self->bounds = (GFC_Rect){ self->position.x, self->position.y, self->sprite->frame_w, self->sprite->frame_h };

	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;

	data = gfc_allocate_array(sizeof(ProjectileData), 1);
	if (data)
	{
		data->direction = dir;
	}
	self->data = data;
	return self;
}

void spawn_projectile(GFC_Vector2D start, ProjectileDir dir, ProjectileNum num) 
{
	start.x = start.x + 12;
	start.y = start.y + 12;
	//if double, double, if triple, triple
	if (num == PN_double)
	{
		Entity* projectile1 = projectile_new(gfc_vector2d(start.x, start.y+7), dir);
		Entity* projectile2 = projectile_new(gfc_vector2d(start.x, start.y-7), dir);
		if (!projectile1 || !projectile2) return;
	}
	else if (num == PN_triple)
	{
		Entity* projectile1 = projectile_new(start, dir);
		Entity* projectile2 = projectile_new(gfc_vector2d(start.x, start.y+12), dir);
		Entity* projectile3 = projectile_new(gfc_vector2d(start.x, start.y-12), dir);
		if (!projectile1 || !projectile2 || !projectile3) return;
	}
	else if (num == PN_all)
	{
		Entity* projectile1 = projectile_new(start, PD_up);
		Entity* projectile2 = projectile_new(start, PD_down);
		Entity* projectile3 = projectile_new(start, PD_left);
		Entity* projectile4 = projectile_new(start, PD_right);
		if (!projectile1 || !projectile2 || !projectile3 || !projectile4) return;
	}
	else
	{
		Entity* projectile = projectile_new(start, dir);
		if (!projectile) return;
	}
}

//void projectile_hit(Entity *self)
//free if it hits something

void projectile_think(Entity* self)
{
	if (!self) return;
	ProjectileData* data = (ProjectileData*)self->data;
	EntitySystem entity_system = entity_get_system();

	switch (data->direction)
	{
		case PD_up:
			self->velocity.y = -5.0;
			break;
		case PD_down:
			self->velocity.y = 5.0;
			break;
		case PD_left:
			self->velocity.x = -5.0;
			break;
		case PD_right:
			self->velocity.x = 5.0;
			break;
		default:
			self->velocity.y = -5.0;
	}
	gfc_vector2d_add(self->position, self->position, self->velocity);

	//check collision with an enemy;
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
				//self->_inuse = 0;  // destroy projectile
				break;
			}
		}
	}
}

void projectile_update(Entity* self)
{
	if (!self) return;
	/*
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;
	*/

	gfc_vector2d_add(self->position, self->position, self->velocity);

	self->bounds = (GFC_Rect){ self->position.x, self->position.y, self->sprite->frame_w, self->sprite->frame_h };

	// Check if projectile is out of screen bounds
	if (self->position.x < 25 || self->position.x > 1200 ||
		self->position.y < 25 || self->position.y > 640)
	{
		projectile_free(self);
	}
		
}

void projectile_free(Entity* self)
{
	ProjectileData* data;
	data = self->data;
	if (!self || !self->data) return;
	free(data);
	self->data = NULL;
	entity_free(self);
}