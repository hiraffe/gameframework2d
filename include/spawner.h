#ifndef __SPAWNER_H__
#define __SPAWNER_H__

#include "entity.h"
#include "world.h"

typedef struct
{
	int			currentIndex;		/*<index of enemy to spawn next*/
	int			maxSimultaneous;	/*<how many enemies can be alive at once*/
	GFC_List*	spawnlist;			/*<list of SpawnInfo*/
	int			totalCount;			/*<total count of current alive enemies*/
	int			finished;			/*<flag if spawning is done*/
	Uint32		alive;				/*<number of enemies currently alive*/
}Spawner;

/**
 * @brief function to get the spawner
 * @return the spawner
 */
Spawner* spawner_get_the();

/**
 * @brief allocate a new spawner
 * @param spawnlist list of enemies to be spawned
 * @return the spawner
 */
Spawner* enemy_spawner_new(GFC_List* spawnlist, int max);

/**
 * @brief update the spawner
 * @param spawner the spawner to update
 * @param world the current world
 */
void enemy_spawner_update(Spawner* spawner, World* world);

/**
 * @brief free a previously allocated spawner
 * @param spawner the spawner to free
 */
void enemy_spawner_free(Spawner* spawner);


#endif