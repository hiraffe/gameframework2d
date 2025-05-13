#ifndef __MENU_H__
#define __MENU_H_

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_list.h"

#include "gf2d_sprite.h"

typedef enum
{
	GS_MainMenu,
	GS_PlayerSelect,
	GS_MainLoop,
	GS_PauseMenu,
	GS_GameOver,
	GS_WinMenu,
	GS_EditorMenu,
	GS_Quit
}GameState;

GameState check_winloss();

int main_menu_update(const Uint8* keys, int mx, int my);
int pause_menu_update(const Uint8* keys, int mx, int my);
int player_select_update(const Uint8* keys, int mx, int my);
int game_over_update(const Uint8* keys, int mx, int my);
int win_menu_update(const Uint8* keys, int mx, int my);

void main_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color);
void pause_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color);
void player_select_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color);
void game_over_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color);
void win_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color);

#endif