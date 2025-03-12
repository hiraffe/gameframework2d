#ifndef __WORLD_H__
#define __WORLD_H__

#include "gfc_text.h"
#include "gfc_vector.h"

#include "gf2d_sprite.h"

typedef struct
{
	GFC_TextLine	name;
	Sprite*			background;		/*<background image for the world*/
	Sprite*			tileSet;		/*<sprite contianing tile images*/
	Uint8*			tileMap;		/*<the tiles that make up the world*/
	Sprite*			tileLayer;		/*<pre rendered tile layer*/
	GFC_Vector2I	tileMapSize;	/*<height (how many tiles high and width (how many tiles long) the map is*/
	//GFC_Vector2I ^^
	//GFC_List		entityList;
}World;

/*
 * @brief load a world from a config file
 * @param filename the name of the world file to load
 * @return NULL on error, or a usable world otherwise
 */
World* world_load(const char *filename);

/**
 * @brief test function to see if world is working
 */
World* world_test_new();

/**
 * @brief allocate a new empty world
 * @return NULL on error, or a blank world
 */
World *world_new(GFC_Vector2I mapSize);
//World* world_new();

/**
 * @brief free a previously allocated world
 * @param world the world to free
 */
 void world_free(World* world);

 /**
  * @brief draw the world
  * @param world the world to draw
  */
 void world_draw(World* world);

#endif