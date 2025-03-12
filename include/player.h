#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

typedef enum
{
	PU_double,
	PU_triple,
	PU_quad,
	PU_speedy,
	PU_reload,
	PU_none
}PowerUp;

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* player_new();

/**
 * @brief get the player to make sure theres no duplicate player
 */
Entity* player_get_the();

/**
 * @brief spawn projectile and have cooldown between shooting each one
 */
void player_attack(Entity* self);

#endif