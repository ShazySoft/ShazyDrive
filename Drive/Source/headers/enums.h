#ifndef ENUMS_H
#define ENUMS_H


///Enumerations -------------------------------------------------------------------

//Textures

enum textures
{
    SPR_NULL,

    SPR_PLAYER,
    SPR_BLOCK,
    SPR_TILE,

    SPR_TOTAL
};

//Sounds and Music

enum sounds
{
    SOUND_NULL,

    SOUND_BOUNCE,

    SOUND_TOTAL
};
Mix_Chunk* sound[SOUND_TOTAL];

enum songs
{
    SONG_NULL,

    SONG_TEST,

    SONG_TOTAL
};
Mix_Music* song[SONG_TOTAL];

//Object Types

enum object_types
{
    OBJ_NULL,

    OBJ_PLAYER,
    OBJ_BLOCK,

    OBJ_TOTAL
};

//Rooms

enum rooms
{
    ROOM_NULL,
    ROOM_TITLE,
    ROOM_MENU,
    ROOM_LEVEL_1,
    ROOM_TOTAL
};

//Menu Buttons

enum menu_buttons
{
    BUTTON_NULL,

    BUTTON_START,

        BUTTON_NEW_GAME,
        BUTTON_LOAD_GAME,

            BUTTON_FILE_1,
            BUTTON_FILE_2,
            BUTTON_FILE_3,

    BUTTON_ARCADE,

    BUTTON_OPTIONS,

        BUTTON_GAMEPLAY,

            SLIDER_DIFFICULTY,
            TOGGLE_AUTOSAVE,

        BUTTON_AUDIOVIDEO,

            SLIDER_VIDEOSCALE,
            SLIDER_SFX,
            SLIDER_MUSIC,

    BUTTON_EXIT,

    BUTTON_TOTAL
};

enum menus
{
    MENU_NULL,

    MENU_TITLE,
    MENU_MAIN,
    MENU_PAUSE,

    MENU_TOTAL
};

//Directions

enum directions
{
    CENTER,

    DOWN,
    UP,
    LEFT,
    RIGHT
};

enum camera_modes
{
    CAMERA_STATIC,
    CAMERA_FOLLOW_PLAYER,
    CAMERA_FOLLOW_PLAYER_X,
    CAMERA_FOLLOW_PLAYER_Y,
    CAMERA_SCROLL_X,
    CAMERA_SCROLL_Y
};

#endif // ENUMS_H
