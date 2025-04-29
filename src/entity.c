#include "simple_logger.h"

#include "gf2d_draw.h"

#include "entity.h"
#include "world.h"

static EntitySystem entity_system = { 0 }; /**<intitalize a LOCAL global entity manager*/

EntitySystem entity_get_system()
{
	return entity_system;
}

void entity_system_close();

void entity_system_init(Uint32 maxEnts)
{
	if (entity_system.entity_list)
	{
		slog("cannot have two instances of an entity system, one is already active");
		return;
	}
	if (!maxEnts)
	{
		slog("cannot initialize entity system for 0 entities");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), maxEnts);
	if (!entity_system.entity_list)
	{
		slog("failed to allocate %i entities", maxEnts);
		return;
	}
	entity_system.entity_max = maxEnts;
	atexit(entity_system_close);
	slog("entity system initialized");
}

void entity_system_close()
{
	if (entity_system.entity_list)
	{
		entity_system_free_all(NULL);
		if (entity_system.entity_list) free(entity_system.entity_list);
		memset(&entity_system, 0, sizeof(EntitySystem));
		//entity_system.entity_list = NULL;
	}
	slog("entity system closed");
}

void entity_system_free_all(Entity* ignore)
{
	int i;
	for (i=0; i<entity_system.entity_max; i++)
	{
		if (&entity_system.entity_list[i] == ignore) continue;
		if (!entity_system.entity_list[i]._inuse) continue; //skip this iteration of the loop
		entity_free(&entity_system.entity_list[i]);
	}
}

Entity* entity_new()
{
	int i;
	for (i=0; i<entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse) continue; //skip entities that are active
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		//set the default color
		//set the default scale
		return &entity_system.entity_list[i];
	}
	slog("no more available entities");
	return NULL;
}

void entity_free(Entity* self)
{
	if (!self) return;
	//anything else we allocate for our entity would get cleaned up here
	self->_inuse = NULL;
	if (self->free) self->free(self);
	if (self->data)
	{
		free(self->data);
		self->data = NULL;
	}
	if (self->sprite)
	{
		gf2d_sprite_free(self->sprite);
		self->sprite = NULL;
	}
}

void entity_think(Entity* self)
{
	if (!self)	return;
	//any boilerplate think stuff here
	if (self->think) self->think(self);
}

void entity_system_think()
{
	int i;
	for (i=0; i<entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_think(&entity_system.entity_list[i]);
	}
}

void entity_update(Entity* self)
{
	if (!self)	return;
	//any boilerplate think stuff here
	if (self->update) self->update(self);
}

void entity_system_update()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_update(&entity_system.entity_list[i]);
	}
}

void entity_draw(Entity* self)
{
	if (!self) return;
	if (!self->sprite) return;

	gf2d_sprite_render(
		self->sprite,
		self->position,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		(Uint32)self->frame);
	
	//if (_DRAWBOUNDS)
	//{
		GFC_Rect rect;
		gfc_rect_copy(rect, self->bounds);
		gfc_vector2d_add(rect, rect, self->position);
		gf2d_draw_rect(self->bounds, GFC_COLOR_RED);
	//}
}

void entity_system_draw()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse) continue;
		entity_draw(&entity_system.entity_list[i]);
	}
}

bool entity_collision(GFC_Rect a, GFC_Rect b)
{
	return !(a.x + a.w < b.x ||
			 a.x > b.x + b.w ||
			 a.y + a.h < b.y ||
			 a.y > b.y + b.h);
}


void entity_move(Entity* self, GFC_Vector2D move)
{
	//gfc_vector2d_add(self->position, self->position, self->velocity);
	//gfc_vector2d_add(self->velocity, self->velocity, self->acceleration);
	//checc for collision

	GFC_Vector2D newPosition;
	gfc_vector2d_add(newPosition, self->position, move);

	if (!tile_is_solid(newPosition))
	{
		self->position = newPosition;
	}
	else
	{
		slog("Blocked by wall!");
	}
}