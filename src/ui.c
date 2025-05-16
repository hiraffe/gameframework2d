#include "simple_logger.h"

#include "ui.h"

void player_draw_ui()
{
	Entity* self = player_get_the();
	PlayerEntityData* data = (PlayerEntityData*)self->data;
	if (!self || !self->data) return;

	float hp_percent = self->health / data->health_max;
	GFC_Rect hp_background = { 40, 650, 80, 16 };
	GFC_Rect hp_foreground = { 40, 650, (int)(80 * hp_percent), 16 };
	GFC_Rect hp_outline = { 40, 650, 80, 16 };

	gf2d_draw_rect_filled(hp_background, gfc_color8(150, 0, 0, 255));
	gf2d_draw_rect_filled(hp_foreground, gfc_color8(0, 255, 0, 255));
	gf2d_draw_rect(hp_outline, gfc_color8(0, 0, 0, 255));

	float tp_percent = (float)data->tp / data->neededtp;
	GFC_Rect tp_background = { 136, 650, 80, 16 };
	GFC_Rect tp_foreground = { 136, 650, (int)(80 * tp_percent), 16 };
	GFC_Rect tp_outline = { 136, 650, 80, 16 };

	gf2d_draw_rect_filled(tp_background, GFC_COLOR_DARKBLUE);
	gf2d_draw_rect_filled(tp_foreground, GFC_COLOR_YELLOW);
	gf2d_draw_rect(tp_outline, gfc_color8(0, 0, 0, 255));
}

void enemy_draw_boss_healthbar()
{
	EntitySystem entity_system = entity_get_system();
	for (int i = 0; i < entity_system.entity_max; i++)
	{
		Entity* enemy = &entity_system.entity_list[i];
		if (!enemy || enemy->team != ETT_monster) continue;
		EnemyEntityData* data = (EnemyEntityData*)enemy->data;
		if (!data) continue;

		if (strcmp(data->name, "girl") == 0)
		{
			if (strcmp(data->name, "girl") == 0)
			{
				float boss_hp_percent = (float)enemy->health / data->health_max;
				//slog("HP: %f / %f = %f\n", enemy->health, data->health_max, boss_hp_percent);
				GFC_Rect boss_hp_background = { enemy->position.x, enemy->position.y-16, 128, 16 };
				GFC_Rect boss_hp_foreground = { enemy->position.x, enemy->position.y-16, (int)(128 * boss_hp_percent), 16 };

				gf2d_draw_rect_filled(boss_hp_background, gfc_color8(150, 0, 0, 255));
				gf2d_draw_rect_filled(boss_hp_foreground, gfc_color8(0, 255, 0, 255));
			}
		}
	}
}


void draw_ui()
{
	player_draw_ui();
	enemy_draw_boss_healthbar();
}