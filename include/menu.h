#ifndef __MENU_H__
#define __MENU_H_

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_list.h"

#include "gf2d_sprite.h"

typedef enum
{
	GS_MainMenu,
	GS_MainLoop,
	GS_PauseMenu,
	GS_EnemyEditor,
	GS_Quit
}GameState;

int main_menu_update(const Uint8* keys, int mx, int my);

void main_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color);

#endif