#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "projectile.h"

typedef enum
{
	PU_double,
	PU_triple,
	PU_quad,
	PU_speedy,
	PU_reload,
	PU_none
}PowerUp;

typedef struct
{
	int xp, neededxp;
	int inventory[10];
	int lastAttack;
	int cooldown;
	int speed;
	PowerUp power;
	int powerExpiry;
}PlayerEntityData;

/**
 * @brief get the player to make sure theres no duplicate player
 * @return the player
 */
Entity* player_get_the();

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* player_new();

/**
 * @brief spawn projectile and have cooldown between shooting each one
 */
void player_attack(Entity* self, ProjectileDir dir);

#endif