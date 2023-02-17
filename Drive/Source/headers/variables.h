#ifndef VARIABLES_H
#define VARIABLES_H

///Variable Declaration

//Engine Variables

bool ENGINE_RUN_WHOLEROOM = 1; //1 = run logic for all objects in room at all times; 2 = run only logic for objects near or in the camera

bool ENGINE_VSYNC = 1; //If vsync is off, the engine will run at ENGINE FPS, and the refresh rate at SCREEN_FPS

int ENGINE_FPS = 180; //Engine speed; -1 for frame-independent engine
int SCREEN_FPS = 60;

//Variables for controlling visual framerate

double FPS_RATIO = ENGINE_FPS / SCREEN_FPS;
int count_fps = 0;
int screen_refresh = 0;

//SDL Screen Variables

double SCREEN_SCALE = 2; //screen upscaling; even numbers look way better

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 360;
int SCREEN_DEPTH = 32;
string WINDOW_NAME = "<ShazyDrive Project>";

SDL_Rect camera_view = {0,0,SCREEN_WIDTH,SCREEN_HEIGHT};

//Bucket Variables

int map_width;
int map_height;

int global_id = 0;

const int bucket_size = 80; //Size of collision buckets; generally, larger = more CPU, smaller = more RAM

int depth_layer_count = 1000; //Amount of individual image layers

//Engine Loop Variables

bool quit_game = 0;
bool game_paused = 0;

bool room_alive = 1;

//Debug Variables

const bool can_debug = 0;
bool debug_mode = 0;

//Tiling Variables

const int tile_width = 64;
const int tile_height = 64;

int tile_id = 0;

//Keyboard Control Variables

enum  keyboard_buttons
{
    keyboard_up,
    keyboard_down,
    keyboard_left,
    keyboard_right,

    keyboard_jump,

    keyboard_primary,
    keyboard_secondary,

    keyboard_pause,

    keyboard_debug,
    keyboard_extra,

    keyboard_total
};

SDL_Keycode key_code[keyboard_total] =
{
    SDLK_w, //up
    SDLK_s, //down
    SDLK_a, //left
    SDLK_d, //right

    SDLK_SPACE, //jump

    SDLK_n, //attack
    SDLK_m, //shield

    SDLK_ESCAPE, //pause

    SDLK_RETURN, //debug
    SDLK_LCTRL //debug delete
};

int key_state[keyboard_total] = {0,0,0,0,0,0,0};

//Mouse Control Variables

enum mouse_buttons
{

    mouse_left_click,
    mouse_right_click,
    mouse_middle_click,
    mouse_button_total
};

Uint32 mouse_code[mouse_button_total] =
{
    SDL_BUTTON_LEFT, //left click
    SDL_BUTTON_RIGHT, //right click
    SDL_BUTTON_MIDDLE //middle click
};

int mouse_state[mouse_button_total] = {0,0,0};

SDL_Point cursor;

#endif // VARIABLES_H
