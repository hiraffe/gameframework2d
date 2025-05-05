#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_graphics.h"

#include "world.h"
#include "enemy.h"

static Entity* theWorld = NULL;

Entity* world_get_the()
{
	return theWorld;
}

void world_tile_layer(World *world)
{
	int i, j;
	Uint32 index;
	Uint32 frame;
	GFC_Vector2D position;
	if (!world) return;

	if (!world->tileSet) return;

	if (world->tileLayer)
	{
		gf2d_sprite_free(world->tileLayer);
	}
	world->tileLayer = gf2d_sprite_new();

	world->tileLayer->surface = gf2d_graphics_create_surface(
		world->tileMapSize.x * world->tileSet->frame_w,
		world->tileMapSize.y * world->tileSet->frame_h);

	world->tileLayer->frame_w = world->tileMapSize.x * world->tileSet->frame_w;
	world->tileLayer->frame_h = world->tileMapSize.y * world->tileSet->frame_h;

	if (!world->tileLayer->surface)
	{
		slog("failed to create tileLayer surface");
		return;
	}

	for (j = 0; j < world->tileMapSize.y; j++)
	{
		for (i = 0; i < world->tileMapSize.x; i++)
		{
			index = i + (j * world->tileMapSize.x);
			if (world->tileMap[index] == 0) continue;

			position.x = i * world->tileSet->frame_w;
			position.y = j * world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;

			gf2d_sprite_draw_to_surface(
				world->tileSet,
				position,
				NULL,
				NULL,
				frame,
				world->tileLayer->surface);
		}
	}
	world->tileLayer->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), world->tileLayer->surface);
	if (!world->tileLayer->texture)
	{
		slog("failed to convert world tile layer to texture");
		return;
	}
}

void world_entities_spawn(World *world, SJson* spawnlist)
{
	int i, count=0;
	SJson* item;
	const char* name;
	const char* enemytype;
	Entity* entity;

	if (!spawnlist) return;
	count = sj_array_get_count(spawnlist);

	for (i = 0; i < count; i++)
	{
		item = sj_array_get_nth(spawnlist, i);

		name = sj_object_get_value_as_string(item, "name");
		if (!name) {
			slog("%s missing 'name' object", spawnlist);
			return NULL;
		}

		enemytype = sj_object_get_value_as_string(item, "enemytype");
		if (!enemytype) {
			slog("%s missing 'enemytype' object", spawnlist);
			return NULL;
		}

		slog("enemy type: %s", enemytype);
		if (strcmp(enemytype, "none") != 0)
		{
			
			slog("enemy spawned");
			entity = enemy_new(enemytype);
		}
		else
		{
			continue;
		}

		gfc_list_append(&world->entityList, entity);
	}
}

World* world_load(const char* filename)
{
	World* world;
	SJson* json = NULL;
	SJson* wjson = NULL;
	SJson* spawnlist = NULL;
	SJson* vertical, * horizontal;
	SJson* item;
	int tile;
	int w = 0, h = 0;
	int i, j;
	const char* tileSet;
	const char* background;
	int frame_w, frame_h, fpl;

	if (!filename)
	{
		slog("no filename provided for world load");
		return NULL;
	}

	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world %s", filename);
		return NULL;
	}

	wjson = sj_object_get_value(json, "world");
	if (!wjson)
	{
		slog("%s missing 'world' object", filename);
		sj_free(json);
		return NULL;
	}

	vertical = sj_object_get_value(wjson, "tileMap");
	if (!vertical)
	{
		slog("%s missing 'tileMap' object", filename);
		sj_free(json);
		return NULL;
	}

	h = sj_array_get_count(vertical);
	horizontal = sj_array_get_nth(vertical,0);
	w = sj_array_get_count(horizontal);

	world = world_new((GFC_Vector2I){w,h});
	if (!world) return NULL;

	for (j=0; j<h; j++)
	{
		horizontal = sj_array_get_nth(vertical, j);
		if (!horizontal) continue;
		for (i=0; i<w; i++)
		{
			item = sj_array_get_nth(horizontal, i);
			if (!item) continue;
			tile = 0;
			sj_get_integer_value(item, &tile);
			world->tileMap[i + (j * w)] = tile;
		}
	}
	background = sj_object_get_value_as_string(wjson, "background");
	world->background = gf2d_sprite_load_image(background);

	tileSet = sj_object_get_value_as_string(wjson, "tileSet");
	sj_object_get_value_as_int(wjson, "frame_w", &frame_w);
	sj_object_get_value_as_int(wjson, "frame_h", &frame_h);
	sj_object_get_value_as_int(wjson, "fpl", &fpl);
	world->tileSet = gf2d_sprite_load_all(
		tileSet,
		frame_w,
		frame_h,
		fpl,
		1);
	world_tile_layer(world);

	spawnlist = sj_object_get_value(wjson, "spawnList");
	if (!spawnlist)
	{
		slog("%s does not contain spawn list", filename);
		sj_free(json);
		return;
	}

	world->entityList = *gfc_list_new();
	world_entities_spawn(world, spawnlist);
	
	sj_free(json);
	theWorld = world;
	return world;
}
/*
World* world_test_new()
{
	int i, width = 75, height = 45;
	GFC_Vector2I size;
	size.x = width;
	size.y = height;
	
	World* world;
	world = world_new(size);
	if (!world) return NULL;

	world->background = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
	world->tileSet = gf2d_sprite_load_all(
		"images/backgrounds/tileset.png",
		16,
		16,
		1,
		1);
	for (i = 0; i < width; i++)
	{
		world->tileMap[i] = 1;
		world->tileMap[i + ((height-1) * width)] = 1;
	}
	for (i = 0; i < height; i++)
	{
		world->tileMap[i*width] = 1;
		world->tileMap[i*width + (width-1)] = 1;
	}
	world_tile_layer(world);
	return world;
}
*/

World *world_new(GFC_Vector2I mapSize)
{
	World* world;
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world)
	{
		slog("failed to allocate a new world");
		return NULL;
	}

	if ((!mapSize.x) || (!mapSize.y))
	{
		slog("cannot make a world with no tile width or height");
		return NULL;
	}
	world->tileMap = gfc_allocate_array(sizeof(Uint8), mapSize.x * mapSize.y);
	if (!world->tileMap)
	{
		slog("failed toallcate memory for tile map");
		free(world);
		return NULL;
	}
	//all boilerplate code would go here
	//all defaults
	world->tileSet = gfc_allocate_array(sizeof(Uint8), mapSize.x * mapSize.y); 
	world->tileMapSize.x = mapSize.x;
	world->tileMapSize.y = mapSize.y;
	return world;
}

void world_free(World* world)
{
	if (!world) return;

	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	gf2d_sprite_free(world->tileLayer);
	if (world->tileMap) free(world->tileMap);
	//spawnlist
	//free every entity in the world
	free(world);
}

void world_draw(World* world)
{
	if (!world) return;
	gf2d_sprite_draw_image(world->background, gfc_vector2d(0,0));
	gf2d_sprite_draw_image(world->tileLayer, gfc_vector2d(0,0));
}


int tile_is_solid(GFC_Vector2D position)
{
	World* world = world_get_the();
	int tileX, tileY, tileIndex, tileValue;

	if (!world)
	{
		slog("could not find world?");
		return 0;
	}

	tileX = (int)(position.x / world->tileSet->frame_w);
	tileY = (int)(position.y / world->tileSet->frame_h);

	// Bounds check
	if (tileX < 0 || tileY < 0 || tileX >= world->tileMapSize.x || tileY >= world->tileMapSize.y) {
		return 1;  // Treat out-of-bounds as solid
	}

	tileIndex = tileX + (tileY * world->tileMapSize.x);
	tileValue = world->tileMap[tileIndex];
	//slog("tile value: %d", tileValue);

	if (tileValue == 1) // wall tile ID
	{
		return 1;
	}

	return 0;
}