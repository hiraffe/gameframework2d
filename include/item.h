#ifndef __ITEM_H__
#define __ITEM_H__

#include "gf2d_sprite.h"

#include "entity.h"
#include "player.h"
#include "world.h"

typedef struct {
    float timer;
    float interval;
    int max_items;
    int items_spawned;
    //SJson* spawnList; // optional: from world JSON
}ItemSpawner;

typedef struct
{
	PowerUp     power;          /*<what power it gives to the player*/
	int         powertime;
    ItemSpawner spawner;
}ItemEntityData;

ItemSpawner item_get_spawner();

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* item_new(PowerUp powerup);

void give_powerup(Entity* self, Entity* player);

void item_spawner_update(ItemSpawner* spawner, World* world, float deltaTime);


#endif