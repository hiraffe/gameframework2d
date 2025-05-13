#include <SDL.h>
#include <SDL_mixer.h>
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
#include "menu.h"
#include "spawner.h"
//#include "particle.h"

GameState game_state = GS_MainMenu;

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
    static Uint32 lastTime = 0;
    static float deltaTime = 0;
    Entity* player;
    ItemSpawner item_spawner;
    Spawner* enemy_spawner = {0};
    GFC_InputController* controller;
    Mix_Chunk* sounds;
    Mix_Music* menu_bg_music;
    
    /*program initializtion*/
    init_logger("gf2d.log",0);
    slog("---==== BEGIN ====---");
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        printf("SDL failed to initialize: %s\n", SDL_GetError());
        return 1;
    }
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
    player_classes_init("defs/player.def");

    SDL_ShowCursor(SDL_DISABLE);

    //gfc_config_def_init();
    //gfc_config_def_load("config/particles.conf");
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16,0);
    slog("press [escape] to quit");
    player = player_new(); //add player
    world = world_load("maps/testworld.map");
    item_spawner = item_get_spawner();
    enemy_spawner = enemy_spawner_new(&world->enemylist, 1);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Mix_OpenAudio failed: %s", Mix_GetError());
    }
    //blaster = MIX_LoadWAV("the sound file"); MIX_LoadMUS
    //MIX_PlayMusic
    menu_bg_music = Mix_LoadMUS("audio/menu-bg-music.mp3"); 
    if (!menu_bg_music)
    {
        slog("Failed to load bg music");
    }
    Mix_PlayMusic(menu_bg_music, -1);
    

    /*main game loop*/
    while(!done)
    {
        /*update things here*/
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        SDL_GetMouseState(&mx, &my);
        mf += 0.1;
        if (mf >= 16.0)mf = 0;
        //if (SDL_GetMouseState(&mx, &my))
        //{
            //particles_from_file("config/spray_particle.particle")
        //}
        
        // get delta time
        Uint32 now = SDL_GetTicks(); 
        deltaTime = (now - lastTime) / 1000.0f; 
        lastTime = now; 

        gf2d_graphics_clear_screen(); // clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame

        switch (game_state)
        {
            case GS_MainMenu:
                main_menu_draw(mx, my, mf, mouse, mouseGFC_Color);
                //slog("pressed: %d", pressed);
                game_state = main_menu_update(keys, mx, my);
                break;
            case GS_PlayerSelect:
                player_select_draw(mx, my, mf, mouse, mouseGFC_Color); 
                game_state = player_select_update(keys, mx, my);
                break;
            case GS_PauseMenu: //change this later to make a pause menu remember!!
                if(Mix_PlayingMusic()) Mix_PauseMusic();
                pause_menu_draw(mx, my, mf, mouse, mouseGFC_Color);
                game_state = pause_menu_update(keys, mx, my); 
                break;
            case GS_MainLoop:
                if (Mix_PausedMusic()) Mix_ResumeMusic();
                entity_system_think();
                entity_system_update();
                //camera_bounds_check();
                item_spawner_update(&item_spawner, world, deltaTime);
                enemy_spawner_update(enemy_spawner, world);
               
                //backgrounds drawn first
                gf2d_sprite_draw_image(sprite, gfc_vector2d(0, 0));
                world_draw(world);

                //entities in the middle
                entity_system_draw();
                //particle_system_draw();

                //UI elements last
                //i just took out the mouse

                if (keys[SDL_SCANCODE_ESCAPE])
                {
                    game_state = GS_PauseMenu; 
                }
                break;
            case GS_Quit:
                done = 1;
                break;
        }

        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame
        
        //if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    Mix_FreeMusic(menu_bg_music);
    Mix_CloseAudio();
    entity_free(player);
    enemies_close();
    player_classes_close();
    enemy_spawner_free(enemy_spawner); 
    world_free(world);
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
