#include "simple_json.h"
#include "simple_logger.h"

#include "gf2d_graphics.h"
//#include "gfc_vector.h"

#include "world.h"

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
	free(world);
}

void world_draw(World* world)
{
	if (!world) return;
	gf2d_sprite_draw_image(world->background, gfc_vector2d(0,0));
	gf2d_sprite_draw_image(world->tileLayer, gfc_vector2d(0, 0));
	/*
	for (j = 0; j < world->tileMapSize.y; j++)
	{
		for (i = 0; i< world->tileMapSize.x; i++)
		{
			index = i + (j * world->tileMapSize.x);
			if (world->tileMap[index] == 0) continue;
			position.x = i*world->tileSet->frame_w;
			position.y = j*world->tileSet->frame_h;
			frame = world->tileMap[index] - 1;
			gf2d_sprite_draw(
				world->tileSet,
				position,
				NULL,
				NULL,
				NULL,
				NULL,
				NULL,
				frame);
		}
	}*/
}

/*
World* world_new()
{
	World* world;
	world = gfc_allocate_array(sizeof(World), 1);
	if (!world)
	{
		slog("ERROR: failed to allocate world");
		return NULL;
	}
	return world;
}
*/
/*
void world_load(const char *filename)
{
	SJson* row;
	int rowCount, columnCount;
	int i, j;
	SJson* column;
	const char* string = NULL;
	SJson* json;
	World* world;
	if (!filename) return NULL;
	json = sj_load(filename);
	if (!json)
	{
		slog("failed to load world %s", filename);
		return NULL;
	}
	world = world_new();
	if (!world) return NULL;
	row = sj_object_get_value(json, "tileMap");
	rowCount = sj_array_get_count(row);
	if (rowCount)
	{
		//row = sj_array_get_nth(row, j);
		if (!row)
		{
			slog("world %s, tileMap missing rows", filename);
			sj_free(json);
			world_free(world);
			return NULL;
		}
		columnCount = sj_array_get_count(row);
		world->tileMap = gfc_allocate_array(sizeof(Uint8), rowCount * columnCount);
		if (world->tileMap)
		{
			slog("failed to allocate tilemap for world %s", filename);
			sj_free(json);
			world_free(world);
			return NULL;
		}
		for (j = 0; j < rowCount; j++)
		{
			row = sj_array_get_nth(row, j);
			if (!row) continue;
			columnCount = sj_array_get_count(row);
			world->tileMapSize.x = columnCount;
			world->tileMapSize.y = rowCount;
			for (i = 0; i < columnCount; i++)
			{
				column = sj_array_get_nth(row, i);
				if (!column) continue;
				sj_get_uint8_value(column, world->tileMap[j * columnCount + i]);
			}
		}
	}

	string = sj_object_get_string(json, "name");
	if (string) gfc_line_cpy(world->name, string);

	string = sj_object_get_string(json, "background");
	if (string)
	{
		world->background = gf2d_sprite_load_image(string);
	}
}

void world_free(World* world)
{
	int i, c;
	Entity* ent;
	if (!world) return;
	gf2d_sprite_free(world->background);
	gf2d_sprite_free(world->tileSet);
	if (world->tileMap) free(world->tileMap);
	if (world->entityList)
	{
		c = gfc_list_count(world->entityList);
		for (i = 0; i < c; i++)
		{
			ent = gfc_list_nth(world->entityList, i);
			if (!ent) continue;
			entity_free(ent);
		}
		gfc_list_delete(world->entityList);
	}
	free(world);
}
*/

