#include "simple_json.h"
#include "simple_logger.h"

#include "spawner.h"

static Spawner* theSpawner = NULL;

Spawner* spawner_get_the()
{
    return theSpawner;
}

Spawner* enemy_spawner_new(GFC_List *spawnlist, int max)
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
    spawner->maxSimultaneous = max;
    spawner->finished = 0;
    spawner->alive = 0;

    slog("spawner created: total enemies = %d", spawner->totalCount);
    theSpawner = spawner;
    return spawner;
}

void enemy_spawner_update(Spawner* spawner, World* world)
{
    if (!spawner || !world || !spawner->spawnlist) return;
    if (spawner->alive >= spawner->maxSimultaneous) return;

    while (spawner->alive < spawner->maxSimultaneous && spawner->currentIndex < spawner->totalCount)
    {
        SpawnInfo* info = gfc_list_get_nth(spawner->spawnlist, spawner->currentIndex++);
        if (!info || !info->name || !info->enemytype) continue;

        Entity* enemy = enemy_new(info->name, info->enemytype);
        if (!enemy) continue;

        gfc_list_append(&world->entityList, enemy);
        spawner->alive++;
        slog("Spawned enemy: %s (%s)", info->name, info->enemytype);
    }

    if (spawner->currentIndex >= spawner->totalCount && spawner->alive < 1);
    {
        spawner->finished = 1;
    }
}

void enemy_spawner_free(Spawner* spawner)
{
    if (!spawner) return;
    // Don't free spawnlist itself if it’s shared externally.
    //gfc_list_clear(spawner->spawnlist);
    free(spawner); 
}