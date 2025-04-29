#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"

typedef enum
{
	MT_wave,	// moves in sine wave
	MT_hbounce,	// bounce between walls horizontal
	MT_down,	// moves straight down
	MT_mfollow,	// follows the mouse
	MT_pfollow	// follows the player
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

void monster_on_hit();

#endif