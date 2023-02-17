#ifndef CLASSES_H
#define CLASSES_H

///Class Declaration

struct point
{
    int x = 0;
    int y = 0;

    bool operator<(const point &thatpoint) const
    {
        if (y < thatpoint.y)
        {
            return 1;
        }

        if (y > thatpoint.y)
        {
            return 0;
        }

        if (y == thatpoint.y)
        {
            if (x <= thatpoint.x)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
};

class savestate
{
public:

    savestate(string file);
    ~savestate();

    bool load();
    void save();

    string filename;
};
savestate* current_save;

class camera
{
public:

    camera(int x,int y,int w, int h);
    ~camera();

    void step();

    SDL_Rect camera_view;
    SDL_Rect active_view;

    SDL_Point camera_go;

    vector<int>active_buckets;
    vector<int>camera_buckets;

    int mode = CAMERA_STATIC;

    int bucket = 0;
    int previous_bucket = 0;

};

class window
{
public:

    window(int x, int y, int w, int h,string window_text);

    ~window();

    void step();

    void handle(SDL_Event& e);

    void move_position(int xhere,int yhere);

    SDL_Rect window_box;

    SDL_Window* my_window;
    SDL_Renderer* my_renderer;
    camera* my_camera;

    int window_id;

    int width;
    int height;
    string text;

    int visible;

    bool has_mouse;
    bool has_keyboard;
};
window* window_game;
window* window_debug;

class timer
{
public:

    timer();


    void start();
    void stop();
    void pause();
    void unpause();


    Uint32 get_ticks();


    bool is_started();
    bool is_paused();


private:


    Uint32 start_ticks;
    Uint32 paused_ticks;

    bool paused;
    bool started;
};

class texture
{
public:

    window* my_window;

    int xoffset;
    int yoffset;

    texture( string path, int spr_width, int spr_height, int image_xoffset, int image_yoffset, window* thiswindow);
    ~texture();

    void load_xpm(int spr_width,int spr_height,int image_xoffset,int image_yoffset);

    //bool load_file(std::string path, int spr_width, int spr_height);
    void draw( double xdraw, double ydraw, int spr_index = 1, double spr_alpha = 1.0, int spr_angle = 0, int center_x = 0, int center_y = 0, SDL_RendererFlip spr_transform = SDL_FLIP_NONE, int img_depth = 0);
    void draw_color( Uint8 red, Uint8 green, Uint8 blue, double xdraw, double ydraw, int spr_index = 1, double spr_alpha = 1.0, int spr_angle = 0, int center_x = 0, int center_y = 0, SDL_RendererFlip spr_transform = SDL_FLIP_NONE, int img_depth = 0);

    void movetowindow(window* thiswindow);

    int get_width();
    int get_height();
    int sprite_width; // width of the clipped sprite
    int sprite_height; // height of the clipped sprite
    int sheet_width;
    int sheet_height;
    int clip_count;
    int sprite_count;

private:

    SDL_Texture* sprite_texture; //pointer to main window texture

    int image_index;
    int img_ind;

    SDL_Point sprite_center; //center point used for rotation

    SDL_Surface* loaded_surface;

    SDL_Rect sprite_clip[600];

};
texture* sprite[SPR_TOTAL];

struct background
{
    background(std::string path, int depth, double hsp,double vsp,double parallax_horizontal,double parallax_vertical, bool loop_horizontally, bool loop_vertically, int snap_horizontally, int snap_vertically, bool dithered, int dither_squaresize);

    ~background();

    void draw();

    texture* sprite_index;

    //Positioning of background

    double x = 0;
    double y = 0;

    //Background scrolling

    double hspeed = 0;
    double vspeed = 0;

    //Parallax effect; the further from zero, the less the background image will try to "follow" the camera
    //A value of 1.0 makes the image stay in the same position in camera, unless a hsp or vsp is specified

    double parallax_horiz = 1;
    double parallax_vert = 1;

    //Looping; if 0, the image will not loop in that direction, but will scroll based on its size and the size of the room
    //Vertical or horizontal looping is preferred for tall or wide levels, respectively

    bool loop_horiz = 1;
    bool loop_vert = 0;

    //Side snap; applies if a background does not loop, but has parallax applied to it
    //The background will scroll off the screen in the direction of this setting when camera moves in the opposite direction
    //Center means it will move towards either side equally

    int snap_horiz = CENTER;
    int snap_vert = CENTER;

    //If the background uses dithering, set this to 1; it will cause the image to snap to a grid based on size of dithered squares in pixels
    //not doing this for dithered backgrounds may result in an annoying "flashing" effect

    bool dither = 0;
    int dither_size = 1;
};

class menu
{
public:
    menu(int menuid,string thisfile,string thisback,string thisfluff,string thisfluff2,string thisfluff3,string thisfluff4);
    ~menu();

    int menu_id;

    void step();
    void draw();

    texture* background;

    SDL_Point background_coord = {0,0};

    map<int,pair<texture*,SDL_Point>> menu_sprite;
    map<int,int>menu_button;
};
menu* current_menu = NULL;

class tile
{
public:
    texture* sprite_index = NULL;

    int depth_offset = 0;

    int sprite_depth = 25;
    int depth_previous = 25;

    double image_index = 1;
    double image_alpha = 1.0;

    int x = 0;
    int y = 0;

    int width = tile_width;
    int height = tile_height;

    int tile_id = 0;

    pair<pair<double,double>,int> tile_key;

    SDL_Rect collision_box;

    tile(double xset,double yset,int index_set,int id,int depth_set);
    ~tile();

    void draw();

    void destroy();

    bool tile_activated = 0;
    void activate();
};

class object
{
public:

    //Identification

    int object_type = OBJ_NULL;
    int object_subtype = 0;

    int object_id = OBJ_NULL;

    //Which window am I in

    window* my_window = window_game;

    //universal functions

    object();
    ~object();

    bool step_active = 1;

    bool alive = 1;

    void basic_step();
    void basic_draw();

    void bucket_place();

    //object-specific functions

    virtual void step() {};
    virtual void draw() {};

    int alarm[5] = {0,0,0,0,0};

    //Bucket Stuff

    int my_bucket = -1;
    int previous_bucket = 0;
    pair<pair<double,double>,int> my_key;

    //Position and Motion

    double x = 0;
    double y = 0;
    double z = 0;

    int xstart = 0;
    int ystart = 0;

    double xprevious = 0;
    double yprevious = 0;

    double hspeed = 0;
    double vspeed = 0;

    double direction = 0;
    double speed = 0;

    double gravity = 0;

    //Collision

    SDL_Rect collision_box = {-999,-999,0,0};

    int collision_xoffset = 0;
    int collision_yoffset = 0;

    bool can_collide = 0;

    //Sprite Variables

    texture* sprite_index = NULL;
    bool visible = 1;
    bool sprite_active = 1;

    double image_index = 1.0;
    double image_speed = 0;
    double image_alpha = 1.0;

    int sprite_xoffset = 0;
    int sprite_yoffset = 0;

    SDL_Point image_range = {1,999};

    int image_xcenter = 0;
    int image_ycenter = 0;
    double image_angle = 0.0;

    SDL_RendererFlip image_transform = SDL_FLIP_NONE;

    int sprite_depth = 50;
    double depth_offset = 0;
    int sprite_depth_previous = -1;

    //Functions

    bool operator==(const object &thisobj)
    {
        return (object_id == thisobj.object_id);
    }

};

class room
{
public:

    //Identification

    int room_id = ROOM_NULL;

    //Basic Functions

    room( int roomid, string roomfile );
    ~room();

    bool load();
    bool save();

    //Engine Functions

    void step();
    void draw();

    void change_size();

    //Variables

    string my_file;

    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;

    int object_count = 0;
    int tile_count = 0;

    //Background stuff

    map< int,unique_ptr<background> >my_background;

};
room* current_room;

int room_go = -1;

class debugger
{
public:

    debugger();
    ~debugger();

    void step();
    void draw();

    int x = 0;
    int y = 0;

    SDL_Rect collision_box;

    int object_type = 1;
};
debugger* main_debugger;


//Specific Object Subclasses

class o_player: public object
{
public:

    o_player( int stype );
    ~o_player();

    void step();
    void draw();
};
o_player* player_pointer = NULL;


class o_block: public object
{
public:

    o_block( int stype );
    ~o_block();

    void draw();

    void step();
};


#endif // CLASSES_H
