#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* projectile_new(GFC_Vector2D position);

#endif