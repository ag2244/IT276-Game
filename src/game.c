#include <SDL.h>

#include <math.h>

#include "simple_logger.h"

#include "gf2d_graphics.h"
#include "gf2d_sprite.h"

#include "starsabove_entity.h"

#include "starsabove_game.h"
#include "starsabove_ui_base.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0; int in_menu = 0;
    const Uint8 * keys;
    const Uint32* mouse;
    Sprite *background_sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouseNormal, *mouseClickable, *mouseSprite;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "Stars Above",
        1200,
        720,
        1500,
        900,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    background_sprite = gf2d_sprite_load_image("images/backgrounds/space.png");
    mouseNormal = gf2d_sprite_load_all("images/ui/cursor.png",32,32,1);
    mouseClickable = gf2d_sprite_load_all("images/ui/cursor_click.png", 32, 32, 1);

    prepare_game();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        mouse = SDL_GetMouseState(&mx,&my);
        //mf+=0.1;
        //if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen between clear_screen and next_frame

        //backgrounds drawn first
        gf2d_sprite_draw_image(background_sprite,vector2d(0,0));
            
            //Stars Above stuff
            processKeys(keys, mouse);

            //starsabove_loop();

            //Mouse position, sprite
            if (starsabove_hoverDetection(mx, my)) {
                mouseSprite = mouseClickable;
            }

            else {
                mouseSprite = mouseNormal;
            }

            gf2d_sprite_draw(
                mouseSprite,
                vector2d(mx, my),
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                0);

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
