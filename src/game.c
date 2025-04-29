#include <SDL.h>
#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gfc_input.h"
#include "gfc_audio.h"

#include "entity.h"
#include "player.h"
#include "monster.h"
#include "world.h"
#include "item.h"
#include "enemy.h"
//#include "particle.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
    World* world;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    GFC_Color mouseGFC_Color = gfc_color8(0,255,255,200);
    Entity* player;
    //Entity* enemy;
    Entity* powerup1, * powerup2, * powerup3, * powerup4, * powerup5;
    GFC_InputController* controller;
    //Mix_Chunk *blaster, Mix_Music
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        gfc_vector4d(0,0,0,255),
        0);
    gfc_audio_init(
        1000,
        128,
        4,
        1,
        1,
        1);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    entity_system_init(1024);
    enemies_init("defs/enemy.def");
    SDL_ShowCursor(SDL_DISABLE);

    //gfc_config_def_init();
    //gfc_config_def_load("config/particles.conf");
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    slog("press [escape] to quit");
    player = player_new(); //add player
    world = world_load("maps/testworld.map");
    
    powerup1 = item_new(PU_double);
    powerup2 = item_new(PU_triple);
    powerup3 = item_new(PU_quad);
    powerup4 = item_new(PU_speedy);
    powerup5 = item_new(PU_reload);
    //enemy = enemy_new();

    //monster_tester();

    //slog();
    //blaster = MIX_LoadWAV("the sound file"); MIX_LoadMUS
    //MIX_PlayMusic

    /*main game loop*/
    while(!done)
    {
        //gfc_input_update(); //hello
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx, &my);
        //if (SDL_GetMouseState(&mx, &my))
        //{
            //particles_from_file("config/spray_particle.particle")
        //}
        mf+=0.1;
        if (mf >= 16.0)mf = 0;

            entity_system_think();
            entity_system_update();
            //camera_bounds_check();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,gfc_vector2d(0,0));
            world_draw(world);

            //entities in the middle
            entity_system_draw();
            //particle_system_draw();
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                gfc_vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseGFC_Color,
                (int)mf);

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    entity_free(player);
    //entity_free(enemy);
    entity_free(powerup1);
    entity_free(powerup2);
    entity_free(powerup3);
    entity_free(powerup4);
    entity_free(powerup5);
    world_free(world);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
