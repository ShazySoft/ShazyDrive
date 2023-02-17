///ShazyDrive v0.1

///Not My Stuff!

//Basic
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <time.h>

//Serialization
#include <fstream>
#include <iostream>

//Containers
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>

//SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


using namespace std;

///My Stuff!

#include "headers/variables.h"
#include "headers/enums.h"

#include "headers/classes.h"
#include "headers/containers.h"

#include "headers/general_functions.h"
#include "headers/class_functions.h"


///Main Function

int main( int argc, char* args[] )
{
    if (init() == 0)
    {
        return 1;
    }
    else
    {
        //Create a main game window and make it visible

        window_create(SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,WINDOW_NAME);
        window_game = windows.back();
        window_game->visible = 1;

        SDL_Event event;

        timer fps;
        timer fps_count;

        int counted_frames = 0;
        double screen_refresh = 0;

        fps_count.start();

        current_save = new savestate("1.sav");

        current_room = new room( ROOM_LEVEL_1,"maps/room_test.map" );

        while (quit_game == 0)
        {
            fps.start();

            for (int i = 0; i < keyboard_total; i++)
            {
                if ( key_state[i] == 1)
                {
                    key_state[i] = 2;
                }

                if ( key_state[i] == 3)
                {
                    key_state[i] = 0;
                }
            }

            for (int i = 0; i < mouse_button_total; i++)
            {
                if ( mouse_state[i] == 1)
                {
                    mouse_state[i] = 2;
                }

                if ( mouse_state[i] == 3)
                {
                    mouse_state[i] = 0;
                }
            }

            while ( SDL_PollEvent(&event) != 0 ) // while event is being polled
            {
                if ( event.type == SDL_QUIT ) // if event detects a quit event
                {
                    quit_game = 1;
                }

                //Quit game if a window is closed

                else if (event.type == SDL_WINDOWEVENT)
                {
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                    {
                        quit_game = 1;
                    }
                }

                else if ( event.type == SDL_KEYDOWN )
                {
                    for (int i = 0; i < keyboard_total; i++)
                    {
                        if ( event.key.keysym.sym == key_code[i] )
                        {
                            key_state[i] = 1;
                        }
                    }
                }

                else if ( event.type == SDL_KEYUP )
                {
                    for (int i = 0; i < keyboard_total; i++)
                    {
                        if ( event.key.keysym.sym == key_code[i] )
                        {
                            key_state[i] = 3;
                        }
                    }
                }

                else if (event.type == SDL_MOUSEMOTION)
                {
                    SDL_GetMouseState( &cursor.x, &cursor.y);
                }

                else if (event.type == SDL_MOUSEBUTTONDOWN)
                {
                    SDL_GetMouseState( &cursor.x, &cursor.y);

                    for (int i = 0; i < mouse_button_total; i++)
                    {
                        if ( event.button.button == mouse_code[i] )
                        {
                            mouse_state[i] = 1;
                        }
                    }
                }
                else if (event.type == SDL_MOUSEBUTTONUP)
                {
                    SDL_GetMouseState( &cursor.x, &cursor.y );

                    for (int i = 0; i < mouse_button_total; i++)
                    {
                        if ( event.button.button == mouse_code[i] )
                        {
                            mouse_state[i] = 3;
                        }
                    }
                }

                for (int i = 0; i < windows.size(); i++)
                {
                    if (windows[i] != NULL)
                    {
                        windows[i]->handle(event);
                    }
                }
            }


            //Window Logic

            for (int i = 0; i < windows.size(); i++)
            {
                if (windows[i] != NULL)
                {
                    SDL_SetRenderDrawColor( windows[i]->my_renderer, 255,255,255,255 );

                    windows[i]->step();

                    SDL_RenderClear(windows[i]->my_renderer); //clear the render screen
                }
            }


            //Room logic

            if (room_alive == 1)
            {
                current_room->step();
            }

            //Changing rooms

            if (room_go != -1)
            {
                delete current_room;

                current_room = new room( ROOM_LEVEL_1,"maps/room_test.map" );

                room_go = -1;
            }

            if (ENGINE_VSYNC == 0)
            {
                if( count_fps > 2000000 )
                {
                    count_fps = 0;
                }
                counted_frames++;

                if ( screen_refresh >= (ENGINE_FPS/SCREEN_FPS) )
                {
                    if (room_alive == 1)
                    {
                        current_room->draw();
                    }

                    for (int i = 0; i < windows.size(); i++)
                    {
                        if ((windows[i] != NULL) & (windows[i]->visible == 1))
                        {
                            SDL_RenderPresent(windows[i]->my_renderer);
                        }
                    }

                    screen_refresh -= (ENGINE_FPS/SCREEN_FPS);
                }
                else
                {
                    screen_refresh++;
                }

                int frameticks = fps.get_ticks();

                if( frameticks < (1000/ENGINE_FPS) )
                {
                    SDL_Delay( (1000/ENGINE_FPS) - frameticks );
                }
            }
            else
            {
                if (room_alive == 1)
                {
                    current_room->draw();
                }

                for (int i = 0; i < windows.size(); i++)
                {
                    if ((windows[i] != NULL) & (windows[i]->visible == 1))
                    {
                        SDL_RenderPresent(windows[i]->my_renderer);
                    }
                }


            }
        }
    }

    quit();

    return 0;
}
