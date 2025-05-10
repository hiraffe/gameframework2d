#ifndef __WORLD_H__
#define __WORLD_H__

#include "simple_json.h"

#include "gfc_text.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gfc_list.h"

#include "gf2d_sprite.h"

typedef struct
{
	char* name;
	char* enemytype;
	//GFC_Vector2D position;
}SpawnInfo;

typedef struct
{
	GFC_TextLine	name;
	Sprite*			background;		/*<background image for the world*/
	Sprite*			tileSet;		/*<sprite contianing tile images*/
	Uint8*			tileMap;		/*<the tiles that make up the world*/
	Sprite*			tileLayer;		/*<pre rendered tile layer*/
	GFC_Vector2I	tileMapSize;	/*<height (how many tiles high and width (how many tiles long) the map is*/
	//GFC_Vector2I ^^
	GFC_List		entityList;		/*<list of entities in the world*/
	GFC_List		*enemylist;		/*<list of enemies in the world*/
	//Spawner*			spawner;ehehehehheahfuahewfuiwaf
}World;

/**
 * @brief get the world
 * @return the world
 */
World* world_get_the();

/**
 * @brief load a world from a config file
 * @param filename the name of the world file to load
 * @return NULL on error, or a usable world otherwise
 */
World* world_load(const char *filename);

/**
 * @brief create world tile layer
 * @param world the world to draw on
 */
void world_tile_layer(World *world);

/**
 * @brief spawn all entities in the world
 * @param spawnlist list of all entities to spawn
 */
void world_load_spawnlist(World* world, SJson *spawnlist);

/**
 * @brief test function to see if world is working
 */
//World* world_test_new();

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

 /** 
  * @brief check if world tile is solid or not
  * @param position where the tile is
  * @return 1 if tile is solid or if position is out of bounds, otherwise 0
  */
 int tile_is_solid(GFC_Vector2D position);

#endif