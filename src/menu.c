#include "simple_logger.h"

#include "menu.h"

static Sprite* menu_bg = NULL;
static Sprite* start_button = NULL;
static Sprite* editor_button = NULL;
static Sprite* quit_button = NULL;

static GFC_Vector4D start_bounds = { 500, 300, 200, 50 };
static GFC_Vector4D editor_bounds = { 500, 370, 200, 50 };
static GFC_Vector4D quit_bounds = { 500, 440, 200, 50 };

static int menu_loaded;

GameState main_menu_update(const Uint8* keys, int mx, int my)
{
    if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        if (mouse_over_button(mx, my, start_bounds))
        {
            return GS_MainLoop;
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
        start_button = gf2d_sprite_load_image("images/buttons/button_test.png");
        editor_button = gf2d_sprite_load_image("images/buttons/button_test.png");
        quit_button = gf2d_sprite_load_image("images/buttons/button_test.png");
    }

    gf2d_sprite_draw_image(menu_bg, gfc_vector2d(0,0));
    gf2d_sprite_draw_image(start_button, gfc_vector2d(500, 300));
    gf2d_sprite_draw_image(editor_button, gfc_vector2d(500, 370));
    gf2d_sprite_draw_image(quit_button, gfc_vector2d(500, 440));

    //draw mouse
    gf2d_sprite_draw(
        mouse,
        gfc_vector2d(mx, my),
        NULL,
        NULL,
        NULL,
        NULL,
        &mouseGFC_Color,
        (int)mf);
}

int mouse_over_button(int mx, int my, GFC_Vector4D button)
{
    return (mx >= button.x && mx <= button.x + button.z &&
        my >= button.y && my <= button.y + button.w);
}