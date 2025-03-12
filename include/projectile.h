#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "entity.h"

typedef enum
{
	PD_up,
	PD_down,
	PD_left,
	PD_right,
	PD_all
}ProjectileDir;

typedef enum
{
	PN_single,
	PN_double,
	PN_triple,
	PN_all
}ProjectileNum; //maybe we can make this a flag instead idk

typedef struct
{
	ProjectileDir direction;
	ProjectileNum type;
}ProjectileData;

/**
 * @brief spawn a new player entity
 * @param position where to spawn it
 * @return NULL on error, or a pointer to the spawned player entity
 */
Entity* projectile_new(GFC_Vector2D position);

#endif