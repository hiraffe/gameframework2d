#ifndef __ENEMY_H__
#define __ENEMY_H_

#include "simple_json.h"

#include "gfc_text.h"

#include "entity.h"
#include "projectile.h"

typedef enum
{
	ET_single,
	ET_double,
	ET_boss
}EnemyType;

typedef struct
{
	GFC_TextLine name;
	int count;
	int monster_max;
	EnemyType type;
}EnemyEntityData;

void monster_tester();

/**
 * @brief free enemy definitions
 */
void enemies_close();

/**
 * @brief initialize and load enemy definitions
 * @param filename the json file containing the enemy data
 */
void enemies_init(const char *filename);

/**
 * @brief get an enemy definition by its name
 * @param name the search criteria
 * @return NULL if not found, otherwise definition information of the enemy
 */
SJson* enemies_get_def_by_name(const char *name);

/**
 * @brief spawn a new player entity
 * @param type what kind of enemy to spawn
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* enemy_new(const char* type);


#endif