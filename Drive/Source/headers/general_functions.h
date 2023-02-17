#ifndef GENERAL_FUNCTIONS_H
#define GENERAL_FUNCTIONS_H

///Function Declaration -----------------------------------------------------------

//Basic Functions

bool collision_basic( SDL_Rect a,SDL_Rect b, int xoffset,int yoffset )
{
    int top_a,bottom_a,left_a,right_a;
    int top_b,bottom_b,left_b,right_b;

    top_a = a.y + yoffset;
    bottom_a = a.y + a.h + yoffset;
    left_a = a.x + xoffset;
    right_a = a.x + a.w + xoffset;


    top_b = b.y;
    bottom_b = b.y + b.h;
    left_b = b.x;
    right_b = b.x + b.w;



    if (bottom_a <= top_b)
    {
        return false;
    }
    if (top_a >= bottom_b)
    {
        return false;
    }
    if (left_a >= right_b)
    {
        return false;
    }
    if (right_a <= left_b)
    {
        return false;
    }

    return true;
}

//Math Functions

int get_random(int beginning = 0, int ending = 1)
{
    int returner = beginning;

    returner += rand() % (abs(ending - beginning) + 1);

    return returner;

}

double get_direction(double xa, double ya, double xb, double yb)
{
    return atan2(ya - yb,xa - xb) + 3.14159;
}

double get_distance(double xa,double ya,double xb,double yb)
{
    return sqrt( pow(xa - xb,2) + pow(ya - yb,2) );
}

double degtorad(double deg)
{
    return deg /180 * 3.14159;
}

double radtodeg(double rad)
{
    return (rad * 180) / 3.14159;
}

//Hash Map Functions

int get_bucket(double xcoord,double ycoord)
{
    if ((xcoord >= 0) & (xcoord <= map_width) & (ycoord >= 0) & (ycoord <= map_height))
    {
        return floor(xcoord / bucket_size) + 1 + (floor(ycoord / bucket_size) * (map_width / bucket_size));
    }
    else
    {
        return 0;
    }

}

void get_buckets(vector<int>& thisvect,SDL_Rect& thisrect, bool room_bounds = 1)
{
    SDL_Rect pullrect;


    if (room_bounds == 1)
    {
        if (thisrect.x > 0)
        {
            pullrect.x = thisrect.x;
        }
        else
        {
            pullrect.x = 0;
        }

        if (thisrect.y > 0)
        {
            pullrect.y = thisrect.y;
        }
        else
        {
            pullrect.y = 0;
        }

        if (thisrect.x + thisrect.w < map_width)
        {
            pullrect.w = thisrect.w;
        }
        else
        {
            pullrect.w = map_width - thisrect.x;
        }

        if (thisrect.y + thisrect.h < map_height)
        {
            pullrect.h = thisrect.h;
        }
        else
        {
            pullrect.h = map_height - thisrect.y;
        }


    }
    else
    {
        pullrect = thisrect;
    }


    int seedblock = get_bucket(pullrect.x,pullrect.y);

    for(int h = 0; h <= floor((pullrect.h + bucket_size)/bucket_size); h++)
    {
        for(int i = 0; i <= floor((pullrect.w + bucket_size)/bucket_size); i++)
        {
            int thisbox = (seedblock + i) + (h * (map_width/bucket_size));
            if (thisbox < collision_map.bucket_count())
            {
                thisvect.push_back(thisbox);
            }

        }
    }

}

//Drawing Functions

void draw_sprite(texture* thistexture, double xpos, double ypos, int index, double alpha = 1, int angle = 0, int xcenter = 0, int ycenter = 0, SDL_RendererFlip transformer = SDL_FLIP_NONE, int depth = 0,double zthis = 0 )
{
    if (thistexture != NULL)
    {
        if (alpha > 1.0)
        {
            alpha = 1.0;
        }

        thistexture->draw(xpos,ypos - zthis,index,alpha,angle,xcenter,ycenter,transformer,depth);
    }
}

void draw_sprite_color(Uint8 rd, Uint8 blu, Uint8 grn, texture* thistexture, double xpos, double ypos, int index, double alpha = 1, int angle = 0, int xcenter = 0, int ycenter = 0, SDL_RendererFlip transformer = SDL_FLIP_NONE, int depth = 0,double zthis = 0 )
{
    if (thistexture != NULL)
    {
        if (alpha > 1.0)
        {
            alpha = 1.0;
        }

        thistexture->draw_color(rd,blu,grn,xpos,ypos - zthis,index,alpha,angle,xcenter,ycenter,transformer,depth);
    }
}

void draw_line_color( window* thiswindow, int xa, int ya,int xb,int yb, int red, int green, int blue, int alpha = 255  )
{
    SDL_SetRenderDrawColor( thiswindow->my_renderer, red, green, blue, alpha );

    SDL_RenderDrawLine( thiswindow->my_renderer, (xa - thiswindow->my_camera->camera_view.x),(ya - thiswindow->my_camera->camera_view.y),(xb - thiswindow->my_camera->camera_view.x),(yb  - thiswindow->my_camera->camera_view.y) );
}

void draw_rectangle( window* thiswindow, SDL_Rect &thisone,int re, int gren, int blu, int a = 255 )
{

    draw_line_color( thiswindow,thisone.x,thisone.y,thisone.x + thisone.w,thisone.y,re,gren,blu,a );
    draw_line_color( thiswindow,thisone.x + thisone.w,thisone.y,thisone.x + thisone.w,thisone.y + thisone.h,re,gren,blu,a );
    draw_line_color( thiswindow,thisone.x,thisone.y + thisone.h,thisone.x + thisone.w,thisone.y + thisone.h,re,gren,blu,a );
    draw_line_color( thiswindow,thisone.x,thisone.y,thisone.x,thisone.y + thisone.h,re,gren,blu,a );
}

void draw_rectangle( window* thiswindow, int xget, int yget, int wget, int hget,int re, int gren, int blu, int a = 255 )
{
    SDL_Rect thisone = {xget,yget,wget,hget};

    draw_line_color( thiswindow,thisone.x,thisone.y,thisone.x + thisone.w,thisone.y,re,gren,blu,a );
    draw_line_color( thiswindow,thisone.x + thisone.w,thisone.y,thisone.x + thisone.w,thisone.y + thisone.h,re,gren,blu,a );
    draw_line_color( thiswindow,thisone.x,thisone.y + thisone.h,thisone.x + thisone.w,thisone.y + thisone.h,re,gren,blu,a );
    draw_line_color( thiswindow,thisone.x,thisone.y,thisone.x,thisone.y + thisone.h,re,gren,blu,a );
}

//Sound Functions

void play_sound(Mix_Chunk* this_sound,int this_volume = 64,int do_loop = 0, int this_channel = -1)
{
    if (this_sound != NULL)
    {
        //Set Volume for Chunk

        Mix_VolumeChunk(this_sound,this_volume);

        //Play chunk

        Mix_PlayChannel(this_channel,this_sound,do_loop);
    }
}

void play_music(Mix_Music* this_song,int this_volume = 64,int do_loop = 0)
{
    if (this_song != NULL)
    {
        //Set Volume for Chunk

        Mix_VolumeMusic(this_volume);

        Mix_PlayMusic(this_song,do_loop);
    }
}

//Object Functions

template<class t>
unique_ptr<t>& instance_create( map<int,unique_ptr<t>>& thismap,int x_start,int y_start,int subtype,double hspstart = 0,double vspstart = 0,int z_start = 0)
{
    int thisid = global_id;

    //Create object in its map file

    thismap.insert( pair<int,unique_ptr<t>>(thisid,unique_ptr<t>(new t(subtype))) );

    //Initialize position and velocity variables

    thismap[thisid]->xstart = x_start;
    thismap[thisid]->ystart = y_start;

    thismap[thisid]->x = x_start;
    thismap[thisid]->y = y_start;

    if (z_start != 0)
    {
        thismap[thisid]->z = z_start;
    }


    thismap[thisid]->hspeed = hspstart;
    thismap[thisid]->vspeed = vspstart;

    thismap[thisid]->collision_box.x = x_start;
    thismap[thisid]->collision_box.y = y_start;

    thismap[thisid]->can_collide = 1;

    //Add object to collision map

    thismap[thisid]->my_key = make_pair(make_pair(x_start,y_start),thisid);

    thismap[thisid]->my_bucket = get_bucket(x_start,y_start);

    thismap[thisid]->bucket_place();

    //Return pointer to object

    //printf("\nID: %i",thisid );

    return thismap[thisid];
}

void instance_destroy( pair<pair<double,double>,int>& this_key )
{
    bool deleter = 1;

    for(int i = 0; i< objects_dead.size(); i++)
    {
        if (objects_dead[i].second == collision_map[this_key])
        {
            deleter = 0;
        }
    }

    if (deleter == 1)
    {
        collision_map[this_key]->alive = 0;
        objects_dead.push_back(make_pair(this_key,collision_map[this_key]));
    }
}

//Keyboard Functions

bool key_check(int whichkey)
{
    if (key_state[whichkey] > 0)
    {
        return 1;
    }
    return 0;
}

bool key_check_press(int whichkey)
{
    bool returner = 0;

    if (key_state[whichkey] == 1)
    {
        returner = 1;
    }
    return returner;
}

bool key_check_release(int whichkey)
{
    bool returner = 0;

    if (key_state[whichkey] == 3)
    {
        returner = 1;
    }
    return returner;
}

//Mouse Functions

bool mouse_check(int whichbutton)
{
    bool returner = 0;

    if (mouse_state[whichbutton] > 0)
    {
        returner = 1;
    }

    return returner;
}

bool mouse_check_press(int whichbutton)
{
    bool returner = 0;

    if (mouse_state[whichbutton] == 1)
    {
        returner = 1;
    }

    return returner;
}

bool mouse_check_release(int whichbutton)
{
    bool returner = 0;

    if (mouse_state[whichbutton] == 3)
    {
        returner = 1;
    }

    return returner;
}

//Tile Functions

tile* tile_create(int xhere,int yhere,int thisimg, int dep = 50)
{
    if ( (xhere + tile_width >= 0) & (yhere + tile_height >= 0) & (xhere <= map_width) & (yhere <= map_height) )
    {
        tile_map.emplace(make_pair(make_pair(xhere,yhere),tile_id),unique_ptr<tile>(new tile(xhere,yhere,thisimg,tile_id,dep)));
        tile_id++;
    }

}

int get_tile(int xthis, int ythis)
{
    if (sprite[SPR_TILE] != NULL)
    {
        return xthis + ( (ythis - 1) * (sprite[SPR_TILE]->sheet_width / tile_width) );
    }
    else
    {
        return -1;
    }
}

//Room Functions

void room_restart()
{
    room_go = current_room->room_id;
}

void room_goto(int thisroom)
{
    room_go = thisroom;
}

///Initialization Function -------------------------------------------------

bool init()
{
    //Seed the RNG

    srand(time(NULL));

    //initialize SDL

    if (SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0)
    {
        return 0;
    }
    else
    {
        //Initialize texture filtering

        if ( !SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0") )
        {
            printf( "Warning: Linear texture filtering not enabled!\n" );
        }

        //Initialize SDL_mixer

        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
            return 0;
        }

        int img_flags = IMG_INIT_PNG;
        if (!(IMG_Init(img_flags)&img_flags))
        {
            printf("SDL_image failed to initialize! SDL_Error: %s\n",SDL_GetError());
            return 0;
        }
    }
    return 1;
}

///Quit Function -----------------------------------------------------------

void quit()
{
    delete current_room;

    for (int i = 0; i < windows.size(); i++)
    {
        if (windows[i] != NULL)
        {
            delete windows[i];
        }
    }

    windows.clear();

    delete current_save;

    IMG_Quit();
    SDL_Quit();
}



#endif // GENERAL_FUNCTIONS_H
