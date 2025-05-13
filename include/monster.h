#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"

typedef enum
{
	MT_smallwave,	// moves in sine wave
	MT_largewave,	// a larger wave
	MT_hbounce,		// bounce between walls horizontal
	MT_down,		// moves straight down
	MT_mfollow,		// follows the mouse
	MT_pfollow		// follows the player
}MonsterType;

typedef struct
{
	const char*	parent_id;	/*<the name of the enemy that spawned the monster*/
	MonsterType type;		/*<behavior type of the monster*/
}MonsterEntityData;

/**
 * @brief spawn a new player entity
 * @param type what kind of monster to spawn
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* monster_new(MonsterType type, const char* parent_id);

Entity* monster_new_position(MonsterType type, const char* parent_id, GFC_Vector2D position);

void monster_free(Entity* self); 

#endif