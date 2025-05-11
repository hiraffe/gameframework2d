#include "simple_json.h"
#include "simple_logger.h"

#include "spawner.h"

static Spawner* theSpawner = NULL;

Spawner* spawner_get_the()
{
    return theSpawner;
}

Spawner* enemy_spawner_new(GFC_List *spawnlist)
{
    if (!spawnlist) return NULL;
    Spawner* spawner = gfc_allocate_array(sizeof(Spawner), 1);
    if (!spawner)
    {
        slog("failed to allocate memory for spawner");
    }

    spawner->spawnlist = spawnlist;
    spawner->totalCount = gfc_list_get_count(spawnlist);
    spawner->currentIndex = 0;
    spawner->maxSimultaneous = 2;
    spawner->finished = 0;
    spawner->alive = 0;

    slog("Spawner created: totalCount = %d", spawner->totalCount);
    theSpawner = spawner;
    return spawner;
}

void enemy_spawner_update(Spawner* spawner, World* world)
{
    if (!spawner || !world || !spawner->spawnlist) return;
    if (spawner->alive >= spawner->maxSimultaneous)
    {
        slog("alive > spawner");
        return;
    }

    while (spawner->alive < spawner->maxSimultaneous && spawner->currentIndex < spawner->totalCount)
    {
        SpawnInfo* info = gfc_list_get_nth(spawner->spawnlist, spawner->currentIndex++);
        if (!info || !info->enemytype) continue;

        Entity* enemy = enemy_new(info->enemytype);
        if (!enemy) continue;

        gfc_list_append(&world->entityList, enemy);
        spawner->alive++;
        //slog("Spawned enemy #%d at (%.1f, %.1f)", spawner->currentIndex - 1, info->position.x, info->position.y);
        //slog("Spawned enemy #%d", spawner->currentIndex - 1);
        slog("Spawned enemy: %s (%s)", info->name, info->enemytype);
    }

    if (spawner->currentIndex >= spawner->totalCount)
    {
        spawner->finished = 1;
        slog("Spawner finished: all enemies spawned and defeated.");
    }
}

void enemy_spawner_free(Spawner* spawner)
{
    if (!spawner) return;
    // Don't free spawnlist itself if it’s shared externally.
    //gfc_list_clear(spawner->spawnlist);
    free(spawner); 
}