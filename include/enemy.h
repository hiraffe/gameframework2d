#ifndef __ENEMY_H__
#define __ENEMY_H_

#include "entity.h"
#include "projectile.h"

typedef struct
{
	int smth;
}EnemyEntityData;

/**
 * @brief get the player to make sure theres no duplicate player
 * @return the player
 */
 //Entity* player_get_the();

 /**
  * @brief spawn a new player entity
  * @param position where to spawn it
  * @return NULL on error, or a pointer to the spawned player entity
  */
Entity* enemy_new();


#endif