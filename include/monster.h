#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "entity.h"

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* monster_new();

#endif