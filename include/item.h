#ifndef __ITEM_H__
#define __ITEM_H__

#include "entity.h"

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* item_new();

#endif