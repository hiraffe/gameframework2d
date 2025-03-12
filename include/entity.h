#ifndef __ENTITY_H__
#define __ENTITY_H__

//#include "simple.json.h"
#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_shape.h"
#include "gf2d_sprite.h"

typedef enum
{
	ETT_none,
	ETT_player,
	ETT_monster,
	ETT_item,
	ETT_MAX
}EntityTeamType;

typedef enum
{
	ECT_entity = 1,
	ECT_world = 2,
	ECT_other = 3,
	ECT_MAX = 7
}EntityCollisionType;

typedef enum
{
	ECL_none = 1,
	ECL_World = 2,
	ECL_Entity = 4,
	ECL_Item = 8,
	ECL_ALL = 15
}EntityCollisionLayers;

typedef struct
{
	float x, y, w, h;
}Bounds;

typedef struct Entity_S
{
	Uint8			_inuse;			/**<flag for keeping track of memroy usage*/
	GFC_TextLine	name;			/**<name of entity for debugging*/
	EntityTeamType  team;			/**<which team is this entity associated with*/
	Uint8			layer;
	GFC_Rect		bounds;
	Sprite*			sprite;			/**<graphical representation og the entity*/
	float			frame;			/**<for drawing the sprite*/
	GFC_Vector2D	position;		/**<where to draw it*/
	GFC_Vector2D	velocity;		/**<how we are moving*/
	GFC_Vector2D	acceleration;	
	float			rotation;
	void			(*think)(struct Entity_S* self);	/**<function to call to make decisions*/
	void			(*update)(struct Entity_S* self);	/**<function to call to execute those decisions*/
	void			(*free)(struct Entity_S* self);	/**<clean up any custom allocated data*/
	void			(*damage)(struct Entity_S* self, struct Entity_S *other, struct Entity_S *credit, float damage, Uint8 damageType);
	void			(*collide)(struct Entity_S* self, struct Entity_S *other, EntityCollisionType type);
	float			health;
	void*			data;			/**<for ad hoc addition data for the entity*/
}Entity;

/**
 * @brief initialize the entity sub entity_system_init
 * @param maxEnts upper limit for how many entities can exist at once
 */
void entity_system_init(Uint32 maxEnts);

/**
 * @brief free all entities in the manager
 * @param ignore do not free this entity
 */
void entity_system_free_all(Entity* ignore);

/**
 * @brief get a new empty entity to work with
 * @return NULL of out of entities, or a blank entity otherwise
 */
Entity* entity_new();

/**
 * @brief get a new empty entity to work with
 * @param self the entity to free
 */
void entity_free(Entity *self);

/**
 * @brief run think functions for all active entities
 */
void entity_system_think();

/**
 * @brief run update functions for all active entities
 */
void entity_system_update();

/**
 * @brief draw all active entities
 */
void entity_system_draw();

#endif