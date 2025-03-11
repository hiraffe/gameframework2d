#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

typedef enum
{
	PT_up,
	PT_down,
	PT_left,
	PT_right,
	PT_all
}ProjectileDir;

typedef enum
{
	PT_single,
	PT_double,
	PT_triple,
	PT_quad
}ProjectileType; //maybe we can make this a flag instead idk

typedef struct
{
	ProjectileDir direction;
	ProjectileType type;
}ProjectileData;

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* projectile_new(GFC_Vector2D position);

#endif