#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"

typedef enum
{
	MT_red,		// follows you
	MT_blue,	// you can't move if it touches you
	MT_orange,	// you have to move if it touches you
	MT_green,	// can't die
	MT_yellow	// normal
}MonsterType;

typedef struct
{
	MonsterType type;
}MonsterEntityData;

/**
 * @brief spawn a bunch of monsters
 */
void monster_tester();

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* monster_new(MonsterType type);

#endif