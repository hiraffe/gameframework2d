#include "simple_logger.h"

#include "item.h"

void item_think(Entity* self);
void item_update(Entity* self);
void item_free(Entity* self);

ItemSpawner spawner = {
	.timer = 0,
	.interval = 2.0f,  // spawn every 2 seconds
	.max_items = 5,
	.items_spawned = 0,
	//.spawnList = spawnlist  // optional: from world JSON
};

ItemSpawner item_get_spawner()
{
	return spawner;
}

void update_item_spawner(ItemSpawner* spawner, World* world, float deltaTime)
{
	if (!spawner || !world) return;
	spawner->timer += deltaTime;

	if (spawner->items_spawned >= spawner->max_items) return;

	if (spawner->timer >= spawner->interval)
	{
		spawner->timer = 0;

		// spawn an enemy
		Entity* item = item_new(PU_double);  // change type dynamically if needed
		if (item)
		{
			ItemEntityData* data = (ItemEntityData*)item->data; 
			//data->spawner = &spawner;
			gfc_list_append(&world->entityList, item);
			spawner->items_spawned++;
			slog("Spawned item #%d", spawner->items_spawned);
		}
	}
}

Entity* item_new(PowerUp powerup)
{
	Entity* self;
	ItemEntityData* data;
	self = entity_new();
	if (!self)
	{
		slog("failed to spawn a new item entity");
		return NULL;
	}
	self->sprite = gf2d_sprite_load_all(
		"images/pointer.png",
		32,
		32,
		16,
		0);
	self->frame = 0;
	self->position = gfc_vector2d( (rand() % (800-50)) +50, (rand() % (600 - 50)) + 50);
	self->bounds = (GFC_Rect){ self->position.x, self->position.y, self->sprite->frame_w, self->sprite->frame_h };

	self->think = item_think;
	self->update = item_update;
	self->free = item_free;

	data = gfc_allocate_array(sizeof(ItemEntityData), 1);
	if (data)
	{
		data->power = powerup;
		data->powertime = 3000;
	}
	self->data = data;

	return self;
}

void give_powerup(Entity* self, Entity* player)
{
	if (!self || !player) return;
	ItemEntityData* data = (ItemEntityData*)self->data;
	PlayerEntityData* p_data = (PlayerEntityData*)player->data;
	Uint32 curr = SDL_GetTicks();

	p_data->power = data->power;
	p_data->powerExpiry = curr + data->powertime;
}

void item_think(Entity* self)
{
	if (!self) return;
	Entity* player = player_get_the();
	ItemSpawner spawner = item_get_spawner();

	// Check for collision
	if (entity_collision(self->bounds, player->bounds)) {
		give_powerup(self, player);
		item_free(self);
		return; // Exit after handling collision
	}
}

void item_update(Entity* self)
{
	if (!self) return;
	self->frame += 0.1;
	if (self->frame >= 16) self->frame = 0;

	gfc_vector2d_add(self->position, self->position, self->velocity);
}

void item_free(Entity* self)
{
	ItemEntityData* data;
	if (!self || !self->data) return;
	data = self->data;
	//other cleanup
	free(data);
	self->data = NULL;
	entity_free(self);
}