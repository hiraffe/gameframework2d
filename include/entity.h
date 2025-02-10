#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "gfc_text.h"
#include "gfc_types.h"
#include "gf2d_sprite.h"

typedef struct Entity_S
{
	Uint8			_inuse;			/**<flag for keeping track of memroy usage*/
	GFC_TextLine	name;			/**<name of entity for debugging*/
	Sprite*			sprite;			/**<graphical representation og the entity*/
	float			frame;			/**<for drawing the sprite*/
	GFC_Vector2D	position;		/**<where to draw it*/
	GFC_Vector2D	velocity;		/**<how we are moving*/
	GFC_Vector2D	acceleration;	
	float			rotation;
	void			(*think)(struct Entity_S* self);	/**<function to call to make decisions*/
	void			(*update)(struct Entity_S* self);	/**<function to call to execute those decisions*/
	void			(*free)(struct Entity_S* self);	/**<clean up any custom allocated data*/
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