#include "simple_logger.h"

#include "menu.h"
#include "player.h"
#include "spawner.h"

static Sprite* menu_bg = NULL;
static Sprite* start_button = NULL;
static Sprite* editor_button = NULL;
static Sprite* resume_button = NULL;
static Sprite* quit_button = NULL;
static Sprite* back_button = NULL;
static Sprite* fighter_button = NULL;
static Sprite* defender_button = NULL;
static Sprite* magician_button = NULL;

static GFC_Vector4D start_bounds = { 500, 300, 200, 50 };
static GFC_Vector4D editor_bounds = { 500, 370, 200, 50 };
static GFC_Vector4D resume_bounds = { 500, 370, 200, 50 };
static GFC_Vector4D quit_bounds = { 500, 440, 200, 50 };
static GFC_Vector4D back_bounds = { 500, 440, 200, 50 };
static GFC_Vector4D fighter_bounds = { 280, 370, 200, 50 };
static GFC_Vector4D defender_bounds = { 500, 370, 200, 50 };
static GFC_Vector4D magician_bounds = { 720, 370, 200, 50 };

static int menu_loaded;

GameState check_winloss()
{
    Entity* player = player_get_the();
    Spawner* spawner = spawner_get_the();
    if (!player || !spawner) return;

    if (player->health <= 0)
    {
        return GS_GameOver;
    }

    if (spawner->dead >= spawner->totalCount)
    {
        return GS_WinMenu;
    }

    return GS_MainLoop;
}

/*============================================================ Main Menu ============================================================ */
GameState main_menu_update(const Uint8* keys, int mx, int my)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        if (mouse_over_button(mx, my, start_bounds))
        {
            return GS_PlayerSelect;
        }
        if (mouse_over_button(mx, my, quit_bounds))
        {
            return GS_Quit;
        }
    }
    return GS_MainMenu;
}

void main_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color)
{
    if (!menu_loaded)
    {
        menu_bg = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
        start_button = gf2d_sprite_load_image("images/buttons/start.png");
        editor_button = gf2d_sprite_load_image("images/buttons/editor.png");
        quit_button = gf2d_sprite_load_image("images/buttons/quit.png");
    }

    gf2d_sprite_draw_image(menu_bg, gfc_vector2d(0,0));
    gf2d_sprite_draw_image(start_button, gfc_vector2d(500, 300));
    gf2d_sprite_draw_image(editor_button, gfc_vector2d(500, 370));
    gf2d_sprite_draw_image(quit_button, gfc_vector2d(500, 440));

    //draw mouse
    gf2d_sprite_draw(mouse, gfc_vector2d(mx, my), NULL, NULL, NULL, NULL, &mouseGFC_Color, (int)mf);
}

/*============================================================ Player Select ============================================================ */
GameState player_select_update(const Uint8* keys, int mx, int my)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        if (mouse_over_button(mx, my, fighter_bounds))
        {
            player_change_class("fighter");
            return GS_MainLoop;
        }
        if (mouse_over_button(mx, my, defender_bounds))
        {
            player_change_class("defender");
            return GS_MainLoop;
        }
        if (mouse_over_button(mx, my, magician_bounds))
        {
            player_change_class("magician");
            return GS_MainLoop;
        }
        if (mouse_over_button(mx, my, quit_bounds))
        {
            return GS_MainMenu;
        }
    }
    return GS_PlayerSelect;
}

void player_select_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color)
{
    if (!menu_loaded)
    {
        menu_bg = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
        fighter_button = gf2d_sprite_load_image("images/buttons/fighter.png");
        defender_button = gf2d_sprite_load_image("images/buttons/defender.png");
        magician_button = gf2d_sprite_load_image("images/buttons/magician.png");
        back_button = gf2d_sprite_load_image("images/buttons/back.png");
    }

    gf2d_sprite_draw_image(menu_bg, gfc_vector2d(0, 0));
    gf2d_sprite_draw_image(fighter_button, gfc_vector2d(280, 370));
    gf2d_sprite_draw_image(defender_button, gfc_vector2d(500, 370));
    gf2d_sprite_draw_image(magician_button, gfc_vector2d(720, 370));
    gf2d_sprite_draw_image(back_button, gfc_vector2d(500, 440));

    //draw mouse
    gf2d_sprite_draw(mouse, gfc_vector2d(mx, my), NULL, NULL, NULL, NULL, &mouseGFC_Color, (int)mf);
}

/*============================================================ Pause Menu ============================================================ */
GameState pause_menu_update(const Uint8* keys, int mx, int my)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        if (mouse_over_button(mx, my, resume_bounds))
        {
            return GS_MainLoop;
        }
        if (mouse_over_button(mx, my, quit_bounds))
        {
            return GS_MainMenu;
        }
    }
    return GS_PauseMenu;
}

void pause_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color)
{
    if (!menu_loaded)
    {
        menu_bg = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
        resume_button = gf2d_sprite_load_image("images/buttons/resume.png");
        back_button = gf2d_sprite_load_image("images/buttons/back.png");
    }

    gf2d_sprite_draw_image(menu_bg, gfc_vector2d(0, 0));
    gf2d_sprite_draw_image(resume_button, gfc_vector2d(500, 370));
    gf2d_sprite_draw_image(back_button, gfc_vector2d(500, 440));

    //draw mouse
    gf2d_sprite_draw(mouse, gfc_vector2d(mx, my), NULL, NULL, NULL, NULL, &mouseGFC_Color, (int)mf);
}

/*============================================================ Game Over ============================================================ */
GameState game_over_update(const Uint8* keys, int mx, int my)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        if (mouse_over_button(mx, my, back_bounds))
        {
            return GS_MainMenu;
        }
        if (mouse_over_button(mx, my, quit_bounds))
        {
            return GS_Quit;
        }
    }
    return GS_GameOver;
}

void game_over_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color)
{
    if (!menu_loaded)
    {
        menu_bg = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
        back_button = gf2d_sprite_load_image("images/buttons/back.png");
        quit_button = gf2d_sprite_load_image("images/buttons/quit.png");
    }

    gf2d_sprite_draw_image(menu_bg, gfc_vector2d(0, 0));
    gf2d_sprite_draw_image(back_button, gfc_vector2d(500, 370));
    gf2d_sprite_draw_image(quit_button, gfc_vector2d(500, 440));

    //draw mouse
    gf2d_sprite_draw(mouse, gfc_vector2d(mx, my), NULL, NULL, NULL, NULL, &mouseGFC_Color, (int)mf);
}

int mouse_over_button(int mx, int my, GFC_Vector4D button)
{
    return (mx >= button.x && mx <= button.x + button.z &&
        my >= button.y && my <= button.y + button.w);
}

/*============================================================ Game Win ============================================================ */
GameState win_menu_update(const Uint8* keys, int mx, int my)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        if (mouse_over_button(mx, my, resume_bounds))
        {
            return GS_MainLoop;
        }
        if (mouse_over_button(mx, my, quit_bounds))
        {
            return GS_MainMenu;
        }
    }
    return GS_WinMenu;
}

void win_menu_draw(int mx, int my, float mf, Sprite* mouse, GFC_Color mouseGFC_Color)
{
    if (!menu_loaded)
    {
        menu_bg = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
        resume_button = gf2d_sprite_load_image("images/buttons/resume.png");
        back_button = gf2d_sprite_load_image("images/buttons/back.png");
    }

    gf2d_sprite_draw_image(menu_bg, gfc_vector2d(0, 0));
    gf2d_sprite_draw_image(resume_button, gfc_vector2d(500, 370));
    gf2d_sprite_draw_image(back_button, gfc_vector2d(500, 440));

    //draw mouse
    gf2d_sprite_draw(mouse, gfc_vector2d(mx, my), NULL, NULL, NULL, NULL, &mouseGFC_Color, (int)mf);
}