#include "simple_json.h"
#include "simple_logger.h"

#include "spawner.h"

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

    slog("Spawner created: totalCount = %d", spawner->totalCount); 
    return spawner;
}

void enemy_spawner_update(Spawner* spawner, World* world)
{
    if (!spawner || !world || !spawner->spawnlist) return;
    slog("total count: %d, current index: %d", spawner->totalCount, spawner->currentIndex);
    int alive = count_alive_enemies(spawner->spawnlist);
    if (alive >= spawner->maxSimultaneous) return;

    while (alive < spawner->maxSimultaneous && spawner->currentIndex < spawner->totalCount)
    //while (spawner->currentIndex < spawner->totalCount)
    {
        SpawnInfo* info = gfc_list_get_nth(spawner->spawnlist, spawner->currentIndex++);
        //spawner->currentIndex++;

        if (!info || !info->enemytype) continue;

        Entity* entity = enemy_new(info->enemytype);
        if (!entity) continue;

        gfc_list_append(&world->entityList, entity);
        alive++;
        //slog("Spawned enemy #%d at (%.1f, %.1f)", spawner->currentIndex - 1, info->position.x, info->position.y);
        //slog("Spawned enemy #%d", spawner->currentIndex - 1);
        slog("Spawned enemy: %s (%s)", info->name, info->enemytype);
        slog("alive: %d", alive);
    }

    if (spawner->currentIndex >= spawner->totalCount)
    {
        spawner->finished = 1;
    }
}

int count_alive_enemies(GFC_List* enemies)
{
    int i, count = 0;
    Entity* enemy;
    for (i = 0; i < gfc_list_get_count(enemies); i++)
    {
        enemy = gfc_list_get_nth(enemies, i);
        if (enemy && enemy->_inuse && enemy->team == ETT_monster)
        {
            count++;
        }
    }
    return count;
}

void enemy_spawner_free(Spawner* spawner)
{
    if (!spawner) return;
    // Don't free spawnlist itself if it’s shared externally.
    free(spawner); 
}