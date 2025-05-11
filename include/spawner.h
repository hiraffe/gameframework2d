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

Spawner* spawner_get_the();
Spawner* enemy_spawner_new(GFC_List* spawnlist);
void enemy_spawner_update(Spawner* spawner, World* world);
int spawner_count_alive(Spawner* spawner);
void enemy_spawner_free(Spawner* spawner);
/**
 * @brief idk
 */
 //Entity* spawn_entity(cont char )

#endif