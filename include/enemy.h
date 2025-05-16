#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "simple_json.h"

#include "gfc_text.h"

#include "entity.h"
#include "monster.h"
#include "spawner.h"
#include "projectile.h"

typedef struct
{
	const char* enemy1;
	const char* enemy2;
	Entity* friend1;
	Entity* friend2;
	int			enemy1_spawned;
	int			enemy2_spawned;
}EnemyBossData;

typedef struct
{
	const char*		name;				/*<name of the kind of enemy*/
	const char*		id;					/*<unique name of the enemy in the world*/
	int				monster_count;		/*<number of current monsters*/
	int				monster_max;		/*<maximum number of monsters allowed*/
	int				delaying;			/*<flag for whether to delay or not*/
	int				delay;				/*<delay between spawning sets of monsters*/
	Uint32			spawn_time;
	float			health_max;
	int				boss;
	EnemyBossData*	boss_data;
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
Entity* enemy_new(const char* type, const char* name);



#endif