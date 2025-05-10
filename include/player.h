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
	const char* special;
	PowerUp power;
	int powerExpiry;
	GFC_Rect nearmiss;
	int tp;
}PlayerEntityData;

/**
 * @brief get the player to make sure theres no duplicate player
 * @return the player
 */
Entity* player_get_the();

/**
 * @brief free player class definitions
 */
void player_classes_close();

/**
 * @brief initialize and load class definitions
 * @param filename the json file containing the class data
 */
void player_classes_init(const char* filename);

/**
 * @brief get a class definition by its name
 * @param name the search criteria
 * @return NULL if not found, otherwise definition information of the class
 */
SJson* player_classes_get_def_by_name(const char* name);

/**
 * @brief spawn a new player entity
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* player_new();

/**
 * @brief change the class of the player entity to one of the classes defined using json
 * @param type which class of player to spawn
 */
void player_change_class(const char* type);

/**
 * @brief spawn projectile and have cooldown between shooting each one
 * @param self the player
 * @param dir the direction of the attack
 */
void player_attack(Entity* self, ProjectileDir dir);

#endif