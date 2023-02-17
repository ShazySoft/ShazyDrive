#ifndef CLASS_FUNCTIONS_H
#define CLASS_FUNCTIONS_H

///Class Functions

//Savestate

savestate::savestate(string file)
{
    filename = file;

    if (load() == 0)
    {

    }
}

savestate::~savestate()
{

}

bool savestate::load()
{
    return 0;
}

void savestate::save()
{

}


//Camera

camera::camera(int x, int y, int w, int h)
{
    camera_view.x = x;
    camera_view.y = y;
    camera_view.w = w;
    camera_view.h = h;

    camera_go.x = x;
    camera_go.y = y;

    active_view.x = x;
    active_view.y = y;
    active_view.w = w * 2;
    active_view.h = h * 2;

    get_buckets(active_buckets,active_view,1);
    get_buckets(camera_buckets,camera_view,1);

    bucket = get_bucket(camera_view.x,camera_view.y);
    previous_bucket = -1;

    mode = CAMERA_FOLLOW_PLAYER;
}

camera::~camera()
{
    active_buckets.clear();
    camera_buckets.clear();
}

void camera::step()
{

    active_view.x = camera_view.x - (camera_view.w / 2);
    active_view.y = camera_view.y - (camera_view.h / 2);

    bucket = get_bucket(camera_view.x,camera_view.y);

    if (previous_bucket != bucket)
    {
        if (active_buckets.size() > 0)
        {
            active_buckets.clear();
        }

        if (camera_buckets.size() > 0)
        {
            camera_buckets.clear();
        }


        get_buckets(active_buckets,active_view,1);

        previous_bucket = bucket;
    }

    //Camera Modes

    if (player_pointer != NULL)
    {
        if (mode == CAMERA_FOLLOW_PLAYER)
        {
            camera_go.x = floor(player_pointer->x - (camera_view.w/2) + (player_pointer->collision_box.w/2));
            camera_go.y = floor(player_pointer->y - (camera_view.h/2) + (player_pointer->collision_box.h/2));
        }

        if (mode == CAMERA_FOLLOW_PLAYER_X)
        {
            camera_go.x = floor(player_pointer->x - (camera_view.w/2) + (player_pointer->collision_box.w/2));
        }

        if (mode == CAMERA_FOLLOW_PLAYER_Y)
        {
            camera_go.y = floor(player_pointer->y - (camera_view.h/2) + (player_pointer->collision_box.h/2));
        }

    }


    //Keep camera go in the room

    if (camera_go.x < 0)
    {
        camera_go.x = 0;
    }

    if (camera_go.y < 0)
    {
        camera_go.y = 0;
    }

    if (camera_go.x > current_room->width - camera_view.w)
    {
        camera_go.x = current_room->width - camera_view.w;
    }

    if (camera_go.y > current_room->height - camera_view.h)
    {
        camera_go.y = current_room->height - camera_view.h;
    }

    //Go to camera_go

    camera_view.x += (camera_go.x - camera_view.x);
    camera_view.y += (camera_go.y - camera_view.y);

    //Keep camera in the room

    if (camera_view.x < 0)
    {
        camera_view.x = 0;
    }

    if (camera_view.y < 0)
    {
        camera_view.y = 0;
    }

    if (camera_view.x > current_room->width - camera_view.w)
    {
        camera_view.x = current_room->width - camera_view.w;
    }

    if (camera_view.y > current_room->height - camera_view.h)
    {
        camera_view.y = current_room->height - camera_view.h;
    }

}


//window

window::window(int x, int y, int w, int h,string window_text)
{
    my_window = SDL_CreateWindow(window_text.c_str(),x,y,w,h,SDL_WINDOW_SHOWN);

    window_id = SDL_GetWindowID(my_window);

    width = w;
    height = h;

    if (ENGINE_VSYNC == 1)
    {
        my_renderer = SDL_CreateRenderer(my_window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    }
    else
    {
        my_renderer = SDL_CreateRenderer(my_window,-1,SDL_RENDERER_ACCELERATED);
    }

    if (SCREEN_SCALE != 1)
    {
        SDL_RenderSetScale(my_renderer,SCREEN_SCALE,SCREEN_SCALE);
        SDL_SetWindowSize(my_window,SCREEN_WIDTH * SCREEN_SCALE,SCREEN_HEIGHT * SCREEN_SCALE);
        SDL_SetWindowPosition(my_window,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
    }

    my_camera = new camera(0,0,w,h);

    window_box.x = x;
    window_box.y = y;
    window_box.w = w;
    window_box.h = h;

    SDL_SetRenderDrawColor(my_renderer,255,255,255,255);
};

window::~window()
{
    SDL_DestroyRenderer(my_renderer);
    my_renderer = NULL;

    SDL_DestroyWindow(my_window);
    my_window = NULL;

    delete my_camera;
}

void window::step()
{




    if (visible == 0)
    {
        SDL_HideWindow(my_window);
    }

    if (visible == 1)
    {
        SDL_ShowWindow(my_window);
    }
}

void window::handle(SDL_Event& e)
{
    if ( (e.type == SDL_WINDOWEVENT) & (e.window.windowID == window_id) )
    {
        switch (e.window.event)
        {
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            has_keyboard = 1;
            break;

        case SDL_WINDOWEVENT_FOCUS_LOST:
            has_keyboard = 0;
            break;

        case SDL_WINDOWEVENT_ENTER:
            has_mouse = 1;
            break;

        case SDL_WINDOWEVENT_LEAVE:
            has_mouse = 0;
            break;

        case SDL_WINDOWEVENT_MOVED:
            SDL_GetWindowPosition(my_window,&window_box.x,&window_box.y);
            break;

        }
    }
}

void window::move_position(int xhere, int yhere)
{
    SDL_SetWindowPosition(my_window,xhere,yhere);
    window_box.x = xhere;
    window_box.y = yhere;
}

window* window_create(int xhere, int yhere, int width, int height,string window_title)
{
    windows.push_back(new window(xhere,yhere,width,height,window_title));

    return windows.back();
}

//Timer

timer::timer()
{
    start_ticks = 0;
    paused_ticks = 0;

    paused = false;
    started = false;
}

void timer::start()
{

    started = true;

    paused = false;

    start_ticks = SDL_GetTicks();

    paused_ticks = 0;

}

void timer::stop()
{
    started = false;

    paused = false;

    start_ticks = 0;

    paused_ticks = 0;
}

void timer::pause()
{
    if ( (started == true) && (paused == false) )
    {
        paused = true;

        paused_ticks = SDL_GetTicks() - start_ticks;

        start_ticks = 0;
    }
}

void timer::unpause()
{
    if ( (started == true) && (paused == true) )
    {
        paused = false;

        start_ticks = SDL_GetTicks() - paused_ticks;

        paused_ticks = 0;
    }
}

Uint32 timer::get_ticks()
{
    Uint32 time = 0;

    if (started == true)
    {
        if (paused == true)
        {
            time = paused_ticks;
        }
        else
        {
            time = SDL_GetTicks() - start_ticks;
        }
    }

    return time;
}

bool timer::is_started()
{
    return started;
}

bool timer::is_paused()
{
    return paused&&started;
}


//Textures

texture::texture( std::string path, int spr_width, int spr_height, int image_xoffset = 0, int image_yoffset = 0, window* thiswindow = window_game )
{
    //Variables

    my_window = thiswindow;

    sprite_texture = NULL;

    sprite_width = spr_width;
    sprite_height = spr_height;
    clip_count = 600;

    xoffset = image_xoffset;
    yoffset = image_yoffset;

    img_ind = -1;

    //Load Image and Create Texture

    loaded_surface = IMG_Load( path.c_str() ); // load the image file 'path'

    if ( loaded_surface == NULL )
    {
        printf("Error loading image file!");
    }
    else
    {
        SDL_SetColorKey( loaded_surface, SDL_TRUE, SDL_MapRGB( loaded_surface->format, 0xFF, 0x00, 0xFF ) ); // make crazy purple alpha
        sprite_texture = SDL_CreateTextureFromSurface( thiswindow->my_renderer,loaded_surface ); // convert the loaded surface to a texture

        if ( sprite_texture != NULL )
        {
            if ( ( spr_width * spr_height ) != 0 ) // if designated size of the sprite clip isn't zero
            {
                sprite_width = spr_width;
                sprite_height = spr_height;
                sprite_count = round((loaded_surface->w * loaded_surface->h) / (sprite_width * sprite_height));
                sheet_width = loaded_surface->w;
                sheet_height = loaded_surface->h;

                for ( int i=0; i<clip_count; i++) // create clip boxes for loaded image, based on given sprite width and height parameters
                {
                    sprite_clip[i].x = ((i-1)-(floor((i-1)/(loaded_surface->w/sprite_width))*(loaded_surface->w/sprite_width)))*sprite_width;
                    sprite_clip[i].y = floor((i-1)/(loaded_surface->w/sprite_width))*sprite_height;
                    sprite_clip[i].w = sprite_width;
                    sprite_clip[i].h = sprite_height;
                }
            }
            else // if designated size of box is set to zero, then the whole image is one clip
            {
                sprite_width = loaded_surface->w;
                sprite_height = loaded_surface->h;

                sprite_clip[1].x = 0;
                sprite_clip[1].y = 0;
                sprite_clip[1].w = sprite_width;
                sprite_clip[1].h = sprite_height;
            }
        }
        else
        {
            printf("Error converting to texture!");
        }

    }
    SDL_SetTextureBlendMode( sprite_texture,SDL_BLENDMODE_BLEND ); // makes alpha transparency work
}

texture::~texture()
{
    SDL_DestroyTexture( sprite_texture );
    sprite_texture = NULL;
    SDL_FreeSurface( loaded_surface ); // delete the loaded surface after it has been used to create clips and texture
}

void texture::draw( double x, double y, int spr_index, double spr_alpha, int spr_angle, int center_x, int center_y, SDL_RendererFlip spr_transform, int img_depth)
{
    if (img_ind == -1)
    {
        image_index = spr_index;
    }
    else
    {
        image_index = img_ind;
    }

    SDL_Rect sprite_box = {round(x - xoffset),round(y - yoffset),sprite_width,sprite_height};
    sprite_center = {center_x,center_y};

    SDL_Rect draw_box = {round(x - xoffset) - camera_view.x,round(y - yoffset) - camera_view.y,sprite_width,sprite_height};  // the actual box of the screen that will be drawn to

    SDL_SetTextureAlphaMod( sprite_texture,round(spr_alpha*255));

    if ( (spr_index<=clip_count) ) // make sure the sprite index exists on the sprite before drawing
    {
        SDL_RenderCopyEx( my_window->my_renderer, sprite_texture, &sprite_clip[image_index], &draw_box, spr_angle, &sprite_center, spr_transform );
    }
    if (spr_index == -99)
    {
        draw_box.w = sheet_width;
        draw_box.h = sheet_height;
        SDL_RenderCopyEx( my_window->my_renderer, sprite_texture, NULL, &draw_box, spr_angle, &sprite_center, spr_transform );
    }

}

void texture::draw_color( Uint8 red, Uint8 green, Uint8 blue, double x, double y, int spr_index, double spr_alpha, int spr_angle, int center_x, int center_y, SDL_RendererFlip spr_transform, int img_depth)
{
    if (img_ind == -1)
    {
        image_index = spr_index;
    }
    else
    {
        image_index = img_ind;
    }

    SDL_Rect sprite_box = {round(x - xoffset),round(y - yoffset),sprite_width,sprite_height};
    sprite_center = {center_x,center_y};

    SDL_Rect draw_box = {(round(x - xoffset)-my_window->my_camera->camera_view.x),(round(y - yoffset)-(my_window->my_camera->camera_view.y)),sprite_width,sprite_height};  // the actual box of the screen that will be drawn to

    SDL_SetTextureAlphaMod( sprite_texture,round(spr_alpha*255));

    SDL_SetTextureColorMod( sprite_texture,red,green,blue );

    if ( (spr_index<=clip_count) ) // make sure the sprite index exists on the sprite before drawing
    {
        SDL_RenderCopyEx( my_window->my_renderer, sprite_texture, &sprite_clip[image_index], &draw_box, spr_angle, &sprite_center, spr_transform );
    }
    if (spr_index == -99)
    {
        draw_box.w = sheet_width;
        draw_box.h = sheet_height;
        SDL_RenderCopyEx( my_window->my_renderer, sprite_texture, NULL, &draw_box, spr_angle, &sprite_center, spr_transform );
    }

    SDL_SetTextureColorMod( sprite_texture,255,255,255 );
}

int texture::get_width()
{
    return sprite_width;
}

int texture::get_height()
{
    return sprite_height;
}

void texture::movetowindow(window* thiswindow)
{
    SDL_DestroyTexture( sprite_texture );
    sprite_texture = NULL;

    my_window = thiswindow;

    sprite_texture = SDL_CreateTextureFromSurface( my_window->my_renderer,loaded_surface ); // convert the loaded surface to a texture
}

//Backgrounds

background::background(std::string filepath, int depth = 1, double hsp = 0,double vsp = 0,double parallax_horizontal = 1.0,double parallax_vertical = 1.0, bool loop_horizontally = 0, bool loop_vertically = 0, int snap_horizontally = CENTER, int snap_vertically = CENTER, bool dithered = 0, int dither_squaresize = 1)
{
    sprite_index = new texture(filepath,0,0);

    hspeed = hsp;
    vspeed = vsp;

    parallax_horiz = parallax_horizontal;
    parallax_vert = parallax_vertical;

    loop_horiz = loop_horizontally;
    loop_vert = loop_vertically;

    snap_horiz = snap_horizontally;
    snap_vert = snap_vertically;

    dither = dithered;
    dither_size = dither_squaresize * 2;
}

background::~background()
{
    if (sprite_index != NULL)
    {
        delete sprite_index;
        sprite_index = NULL;
    }

}

void background::draw()
{
    if (sprite_index != NULL)
    {
        x += hspeed;
        y += vspeed;

        SDL_Rect location;

        //handle positioning of tiles

        if (loop_horiz == 1)
        {
            location.x = (camera_view.x / parallax_horiz) + x;
        }
        else
        {
            location.x = camera_view.x + x - ((sprite_index->sprite_width - camera_view.w) * ((camera_view.x + camera_view.w) / current_room->width ));

            if (snap_horiz == LEFT)
            {
                location.x -= (camera_view.x / 2) * (parallax_horiz - 1);
            }
            if (snap_horiz == RIGHT)
            {
                location.x += ((current_room->width - camera_view.x - camera_view.w) / 40) * (parallax_horiz - 1);
            }
        }

        if (loop_vert == 1)
        {
            location.y = (camera_view.y / parallax_vert) + y;
        }
        else
        {
            location.y = camera_view.y + y - ((sprite_index->sprite_height - SCREEN_HEIGHT) * ((camera_view.y + camera_view.h) / (current_room->height) ));

            if (snap_vert == UP)
            {
                location.y -= (camera_view.y / 2) * (parallax_vert - 1);
            }
            if (snap_vert == DOWN)
            {
                location.y += ((current_room->height - camera_view.y - camera_view.h) / 40) * (parallax_vert - 1);
            }
        }

        int loop_x = 0;
        int loop_y = 0;

        if (loop_horiz == 1)
        {
            loop_x = ceil(SCREEN_WIDTH / sprite_index->sprite_width) + 2;
        }

        if (loop_vert == 1)
        {
            loop_y = ceil(SCREEN_WIDTH / sprite_index->sprite_height) + 2;
        }

        for (int i = 0; i < loop_y; i++)
        {
            for (int f = 0; f < loop_x; f++)
            {
                SDL_Point location_adjust = {location.x,location.y};

                if (loop_horiz == 1)
                {
                    location_adjust.x = (location.x) + ((f - 1) * sprite_index->sprite_width);

                    if (location_adjust.x < camera_view.x - sprite_index->sprite_width)
                    {
                        location_adjust.x += sprite_index->sprite_width * loop_x;
                    }

                    if (location_adjust.x > camera_view.x + camera_view.w)
                    {
                        location_adjust.x -= sprite_index->sprite_width * loop_x;
                    }
                }

                if (loop_vert == 1)
                {
                    location_adjust.y = (location.y) + ((i - 1) * sprite_index->sprite_height);

                    if (location_adjust.y < camera_view.y - sprite_index->sprite_height)
                    {
                        location_adjust.y += sprite_index->sprite_height * loop_y;
                    }

                    if (location_adjust.y > camera_view.y + camera_view.h)
                    {
                        location_adjust.y -= sprite_index->sprite_height * loop_y;
                    }
                }

                if (dither == 1)
                {
                    location_adjust.x -= location_adjust.x % dither_size + (camera_view.x % dither_size);
                    location_adjust.y -= location_adjust.y % dither_size + (camera_view.y % dither_size);
                }

                draw_sprite(sprite_index,location_adjust.x,location_adjust.y,1);
            }
        }

    }
}

//Menu

menu::menu(int menuid, string thisfile,string thisback,string thisfluff = "",string thisfluff2 = "",string thisfluff3 = "",string thisfluff4 = "")
{
    menu_id = menuid;

    background = new texture(thisback.c_str(),0,0);

    SDL_Point coord = {-999,-999};

    if (thisfluff != "")
    {
        menu_sprite.emplace(1,make_pair(new texture(thisfluff.c_str(),0,0),coord));
    }

    if (thisfluff2 != "")
    {
        menu_sprite.emplace(2,make_pair(new texture(thisfluff2.c_str(),0,0),coord));
    }

    if (thisfluff3 != "")
    {
        menu_sprite.emplace(3,make_pair(new texture(thisfluff3.c_str(),0,0),coord));
    }

    if (thisfluff4 != "")
    {
        menu_sprite.emplace(4,make_pair(new texture(thisfluff4.c_str(),0,0),coord));
    }

    if (menu_id == MENU_TITLE)
    {
        menu_sprite[1].second.x = 10;
        menu_sprite[1].second.y = 10;

        menu_sprite[2].second.x = 110;
        menu_sprite[2].second.y = 0;
    }
}

menu::~menu()
{
    delete background;
    background = NULL;

    menu_sprite.clear();
}

void menu::draw()
{

    if (menu_id == MENU_MAIN)
    {
        if (menu_sprite[1].second.x < 200)
        {
            menu_sprite[1].second.x += 1;
        }
    }

    background_coord.x -= 2;
    background_coord.y += 1;

    if (background != NULL)
    {
        for (int i = -1; i < SCREEN_WIDTH / background->sprite_width + 2; i++ )
        {
            for (int j = -1; j < SCREEN_HEIGHT / background->sprite_height + 2; j++ )
            {
                draw_sprite(background,(background->sprite_width * i) + (background_coord.x % background->sprite_width),(background->sprite_height * j) + (background_coord.y % background->sprite_height),1);
            }
        }


    }

    for (auto i = menu_sprite.begin(); i != menu_sprite.end(); i++)
    {
        draw_sprite(i->second.first,i->second.second.x,i->second.second.y,1);
    }
}

void menu::step()
{

}


//Tiles

tile* tile_collision(SDL_Rect thisrect,int xoffset, int yoffset, int thisdep = -1)
{
    tile* returner = NULL;

    //If the collision box being checked is within the bounds of the spatially hashed tile map, only check activated tiles

    if (collision_basic(thisrect,window_game->my_camera->camera_view,0,0))
    {
        for (auto i = active_tiles.begin(); i != active_tiles.end(); i++)
        {
            if ( collision_basic(thisrect,i->second->collision_box,xoffset,yoffset) )
            {
                if ( thisdep == -1 )
                {
                    returner = i->second;
                }
                else
                {
                    if (i->second->depth_offset == thisdep)
                    {
                        returner = i->second;
                    }
                }
            }
        }
    }
    else
    {
        for (auto i = tile_map.begin(); i != tile_map.end(); i++)
        {
            if ( collision_basic(thisrect,i->second->collision_box,xoffset,yoffset) )
            {
                if ( thisdep == -1 )
                {
                    returner = i->second.get();
                }
                else
                {
                    if (i->second->depth_offset == thisdep)
                    {
                        returner = i->second.get();
                    }
                }
            }
        }
    }

    return returner;
}

tile::tile(double xset,double yset,int index_set,int id,int depth_set = 0)
{
    x = xset;
    y = yset;

    if (sprite[SPR_TILE] == NULL)
    {
        sprite[SPR_TILE] = new texture("sprites/biomes/spr_grassland.png",tile_width,tile_height);
    }
    sprite_index = sprite[SPR_TILE];

    image_index = index_set;

    depth_offset = depth_set;

    if (image_index == get_tile(9,4))
    {
        depth_offset = 3;
    }

    if (image_index == get_tile(9,5))
    {
        depth_offset = 2;
    }

    tile_id = id;



    collision_box.x = x;
    collision_box.y = y;
    collision_box.w = tile_width;
    collision_box.h = tile_height;

    tile_key = make_pair(make_pair(collision_box.x,collision_box.y),id);

    sprite_depth = floor((collision_box.y + (collision_box.h / 2)) + (depth_offset * tile_height)) / (tile_height / 2);

    if (sprite_depth < 0)
    {
        sprite_depth = 0;
    }

    if (sprite_depth > depth_layer_count)
    {
        sprite_depth = depth_layer_count;
    }

    depth_previous = -1;

    tile_activated = 0;

    if (debug_mode == 1)
    {
        activate();
    }

}

tile::~tile()
{

}

void tile::activate()
{
    sprite_depth = floor((collision_box.y + (collision_box.h / 2)) + (depth_offset * tile_height)) / (tile_height / 2);

    if (tile_activated == 0)
    {
        if (collision_basic(collision_box,window_game->my_camera->camera_view,0,0) )
        {
            if (sprite_depth != -1)
            {
                active_tiles.emplace(make_pair(sprite_depth,tile_id),this);
            }
            tile_activated = 1;
        }
    }
    else
    {
        if (!collision_basic(collision_box,window_game->my_camera->camera_view,0,0) )
        {
            auto findy = active_tiles.find( make_pair(sprite_depth,tile_id) );

            if (findy != active_tiles.end() )
            {
                active_tiles.erase(findy);
            }
            tile_activated = 0;
        }
    }
}

void tile::draw()
{
    if (sprite_index != NULL)
    {
        draw_sprite(sprite_index,x,y,image_index,1,0,0,0,SDL_FLIP_NONE,sprite_depth);
    }
}


void tile::destroy()
{
    auto findy = active_tiles.find( make_pair(depth_previous,tile_id) );

    if (findy != active_tiles.end() )
    {
        active_tiles.erase(findy);
    }

    auto findo = active_tiles.find( make_pair(sprite_depth,tile_id) );

    if (findo != active_tiles.end() )
    {
        active_tiles.erase(findo);
    }

    tiles_dead.push_back(tile_key);
}


//Basic Object

object::object()
{
    object_id = global_id;

    object_map.insert(make_pair(object_id,this));

    global_id++;
}

object::~object()
{
    visible = 0;

    auto findy = active_sprites.find(make_pair(sprite_depth,object_id));

    if (findy != active_sprites.end() )
    {
        active_sprites.erase(findy);
    }
    auto finderoni = active_sprites.find(make_pair(sprite_depth_previous,object_id));

    if (finderoni != active_sprites.end() )
    {
        active_sprites.erase(finderoni);
    }

    auto finder = collision_map.find(my_key);

    if ( finder != collision_map.end() )
    {
        collision_map.erase(my_key);
    }

}

void object::basic_step()
{
    //Sprite Bucket Stuff

    sprite_depth = floor((collision_box.y + (collision_box.h / 2)) + depth_offset / (tile_height / 2));

    if (sprite_depth < 0)
    {
        sprite_depth = 0;
    }
    if (sprite_depth > depth_layer_count - 1)
    {
        sprite_depth = depth_layer_count - 1;
    }

    if (visible == 1)
    {
        //If Visible and in Camera, Add Active Texture

        if (sprite_active == 0)
        {
            if (collision_basic(collision_box,my_window->my_camera->active_view,0,0) == 1 )
            {
                active_sprites.emplace(make_pair(sprite_depth,object_id),this);

                sprite_active = 1;
            }
        }

        //If visible but out of Camera, Delete Active Texture

        if (sprite_active == 1)
        {
            if (collision_basic(collision_box,my_window->my_camera->active_view,0,0) == 0 )
            {
                auto findy = active_sprites.find(make_pair(sprite_depth,object_id));

                if (findy != active_sprites.end() )
                {
                    active_sprites.erase(findy);
                }

                sprite_active = 0;
            }
        }

        //If Sprite Depth Changes, Update Active Texture

        if (sprite_depth_previous != sprite_depth)
        {
            auto findy = active_sprites.find(make_pair(sprite_depth_previous,object_id));

            if (findy != active_sprites.end() )
            {
                active_sprites.erase(findy);
            }

            active_sprites.emplace(make_pair(sprite_depth,object_id),this);

            sprite_depth_previous = sprite_depth;
        }

    }
    else
    {
        //Delete Active Sprite Object when not Visible

        if (sprite_active == 1)
        {
            auto findy = active_sprites.find(make_pair(sprite_depth,object_id));

            if (findy != active_sprites.end() )
            {
                active_sprites.erase(findy);
            }

            sprite_active = 0;
        }

    }

    //General Sprite Stuff

    image_index += (image_speed/4);

    if (image_index < image_range.x)
    {
        image_index = image_range.x;
    }

    if (image_index > image_range.y+1-(image_speed/2))
    {
        image_index = image_range.x;
    }
    if (image_range.x == image_range.y)
    {
        image_index = image_range.x;
    }

    //motion Stuff

    x += hspeed / (FPS_RATIO);
    y += vspeed / (FPS_RATIO);

    vspeed += gravity / (FPS_RATIO);

    xprevious = x;
    yprevious = y;

    collision_box.x = round(x) + collision_xoffset;
    collision_box.y = round(y) + collision_yoffset;

    //Alarm Countdown

    for (int i = 0; i < 5; i++)
    {
        if (alarm[i] > 0)
        {
            alarm[i] -= 1;
        }
    }

    step();

}

void object::basic_draw()
{
    if (visible == 1)
    {
        if (sprite_index != NULL)
        {
            draw_sprite( sprite_index,x + sprite_xoffset, y + sprite_yoffset, image_index,image_alpha,image_angle,image_xcenter,image_ycenter,image_transform,sprite_depth,z);
        }

        //draw_rectangle(my_window,collision_box,255,255,255);

        draw();
    }
}

void object::bucket_place()
{
    //Bucket Stuff

    if ((hspeed != 0) | (vspeed != 0))
    {
        my_bucket = get_bucket(collision_box.x,collision_box.y);
    }

    //Bucket Placement

    if (previous_bucket != my_bucket)
    {
        //Delete old collision map item

        auto finder = collision_map.find(my_key);

        if ( finder != collision_map.end() )
        {
            collision_map.erase(my_key);
        }

        my_key = make_pair(make_pair(collision_box.x,collision_box.y),object_id);

        if (ENGINE_RUN_WHOLEROOM == 0)
        {
            //Player and Boss objects are placed in the zero bucket, which always runs

            if ( (object_type == OBJ_PLAYER) )
            {
                my_key = make_pair(make_pair(-90,-90),object_id);
            }
        }

        collision_map.emplace(my_key,this);

        previous_bucket = my_bucket;
    }
}

template<class u>
vector<u*> collision_check(double xofsettt,double yofsettt,SDL_Rect& mybox,map<int,unique_ptr<u>>& this_obj)
{
    vector<u*> returner;

    vector<int>theseboxes;

    SDL_Rect col_box;

    col_box.x = mybox.x - (bucket_size*1.5);
    col_box.y = mybox.y - (bucket_size*1.5);
    col_box.w = mybox.w + (bucket_size*1.5);
    col_box.h = mybox.h + (bucket_size*1.5);

    get_buckets(theseboxes,col_box);

    for (auto i = theseboxes.begin(); i!=theseboxes.end(); i++)
    {
        for (auto f = collision_map.begin(*i); f != collision_map.end(*i); f++)
        {
            if (collision_basic(mybox,f->second->collision_box,xofsettt,yofsettt) == true)
            {
                if (f->second->can_collide == 1)
                {
                    returner.push_back( dynamic_cast<u*>(f->second) );
                }
            }
        }
    }


    for (auto f = collision_map.begin(0); f != collision_map.end(0); f++)
    {
        if (collision_basic(mybox,f->second->collision_box,xofsettt,yofsettt) == true)
        {
            if (f->second->can_collide == 1)
            {
                returner.push_back( dynamic_cast<u*>(f->second) );
            }
        }
    }

    theseboxes.clear();

    return returner;

}


//Player Object

o_player::o_player( int stype )
{
    if (sprite[SPR_PLAYER] == NULL)
    {
        sprite[SPR_PLAYER] = new texture("spr_circle.png",32,32);
    }
    sprite_index = sprite[SPR_PLAYER];

    object_type = OBJ_PLAYER;

    player_pointer = this;

    my_window = window_game;

    my_window->my_camera->mode = CAMERA_FOLLOW_PLAYER;

    collision_box.w = 32;
    collision_box.h = 32;
}

o_player::~o_player()
{

}

void o_player::step()
{
    if (key_check(keyboard_jump))
    {
        room_restart();
    }

    if (key_check(keyboard_left))
    {
        x -= 5;
    }
    if (key_check(keyboard_right))
    {
        x += 5;
    }
    if (key_check(keyboard_up))
    {
        y -= 5;
    }
    if (key_check(keyboard_down))
    {
        y += 5;
    }
}

void o_player::draw()
{

}


//Block Object

o_block::o_block( int stype )
{
    object_type = OBJ_BLOCK;

    if (sprite[SPR_BLOCK] == NULL)
    {
        sprite[SPR_BLOCK] = new texture("sprites/spr_debug_blocks.png",tile_width,tile_height);
    }

    collision_box.w = tile_width;
    collision_box.h = tile_height;

    depth_offset = 9;

    sprite_depth = floor((collision_box.y + (collision_box.h / 2)) + (depth_offset * tile_height)) / (tile_height / 2);

    if ( debug_mode == 1 )
    {
        active_sprites.emplace(make_pair(sprite_depth,object_id),this);

        sprite_active = 1;
    }

    //Change tile depths, if applicable

    if (z != 0)
    {
        int tilestochange = (z / tile_height);

        for (int j = 0; j < tilestochange; j++)
        {
            if (tile_collision(collision_box,0,-tilestochange * tile_height) != NULL)
            {
                tile_collision(collision_box,0,-tilestochange * tile_height)->depth_offset = tilestochange + 1;
                tile_collision(collision_box,0,-tilestochange * tile_height)->activate();
            }
        }
    }

}

o_block::~o_block()
{

}

void o_block::draw()
{

}

void o_block::step()
{
    visible = 1; //debug_mode;
    //printf("\nMy Z: %f",z);
    image_alpha = 0.2;
}


//Room

bool room::save()
{

    ofstream file;

    SDL_RWops* testfile = SDL_RWFromFile(my_file.c_str(), "w+b" );

    if (testfile == NULL)
    {
        printf("\nFailed to load room file!");
        return 0;
    }

    int o_count = static_cast<int>(collision_map.size() );

    int t_count = static_cast<int>(tile_map.size() );

    SDL_RWwrite( testfile,&width,sizeof(int),1);
    SDL_RWwrite( testfile,&height,sizeof(int),1);

    SDL_RWwrite( testfile,&o_count,sizeof(int),1);
    SDL_RWwrite( testfile,&t_count,sizeof(int),1);

    for ( auto save_iterator = collision_map.begin(); save_iterator!=collision_map.end(); save_iterator++ )
    {
        int obj = save_iterator->second->object_type;
        int stype = save_iterator->second->object_subtype;
        int xset = save_iterator->second->xstart;
        int yset = save_iterator->second->ystart;

        SDL_RWwrite( testfile,&obj,sizeof(int),1);
        SDL_RWwrite( testfile,&stype,sizeof(int),1);
        SDL_RWwrite( testfile,&xset,sizeof(int),1);
        SDL_RWwrite( testfile,&yset,sizeof(int),1);
    }

    for ( auto save_tiles = tile_map.begin(); save_tiles != tile_map.end(); save_tiles++ )
    {
        int ind = static_cast<int>(save_tiles->second->image_index);
        int xset = static_cast<int>(save_tiles->second->x);
        int yset = static_cast<int>(save_tiles->second->y);
        int dep = static_cast<int>(save_tiles->second->depth_offset);

        SDL_RWwrite( testfile,&ind,sizeof(int),1);
        SDL_RWwrite( testfile,&xset,sizeof(int),1);
        SDL_RWwrite( testfile,&yset,sizeof(int),1);
        SDL_RWwrite( testfile,&dep,sizeof(int),1);
    }

    SDL_RWclose( testfile );

    printf( "\nRoom Saved!" );

    return 0;
}

bool room::load()
{
    SDL_RWops* testfile = SDL_RWFromFile(my_file.c_str(), "r+b" );

    if (testfile == NULL)
    {
        printf("\nFailed to load room file!");
        return 0;
    }

    try
    {
        int obj_count;

        SDL_RWread( testfile,&width,sizeof(int),1);
        SDL_RWread( testfile,&height,sizeof(int),1);

        width = 3000;
        height = 2000;

        SDL_RWread( testfile,&obj_count,sizeof(int),1);
        SDL_RWread( testfile,&tile_count,sizeof(int),1);

        if (width % bucket_size!= 0)
        {
            width = width - (width % bucket_size) + bucket_size;
        }

        if (height % bucket_size != 0)
        {
            height = height - (height % bucket_size) + bucket_size;
        }

        map_width = width;
        map_height = height;

        collision_map.max_load_factor(10000);
        collision_map.rehash( ((width * height) / (bucket_size * bucket_size)) );

        depth_layer_count = floor(height / tile_height) * 4;

        active_sprites.max_load_factor(10000);
        active_sprites.rehash(depth_layer_count);

        tile_map.rehash( ((width * height) / (bucket_size * bucket_size)) );

        active_tiles.max_load_factor(10000);
        active_tiles.rehash(depth_layer_count);

        for( int i = 0; i < obj_count; i++ )
        {
            int obj,subtype;

            int xset,yset;

            SDL_RWread( testfile,&obj,sizeof(int),1);
            SDL_RWread( testfile,&subtype,sizeof(int),1);
            SDL_RWread( testfile,&xset,sizeof(int),1);
            SDL_RWread( testfile,&yset,sizeof(int),1);

            if (obj == OBJ_PLAYER)
            {
                if (obj_player.size() == 0)
                {
                    instance_create( obj_player,xset,yset,1 );
                }
            }

            if (obj == OBJ_BLOCK)
            {
                instance_create( obj_block,xset,yset,subtype );
            }

        }

        for ( int j = 0; j < tile_count; j++ )
        {
            int til_ind;

            int xset,yset;

            int dep;

            SDL_RWread( testfile,&til_ind,sizeof(int),1);
            SDL_RWread( testfile,&xset,sizeof(int),1);
            SDL_RWread( testfile,&yset,sizeof(int),1);
            SDL_RWread( testfile,&dep,sizeof(int),1);

            tile_create(xset,yset,til_ind,dep);
        }
    }

    catch(...)
    {
        printf("\nSomething Messed Up!");
        SDL_RWclose(testfile);
        return 0;
    }

    SDL_RWclose(testfile);

    return 1;
}

room::room( int roomid, string roomfile )
{
    game_paused = 0;

    room_id = roomid;

    my_file = roomfile;

    if (can_debug == 1)
    {
        main_debugger = new debugger();
    }

    if (load() == 0)
    {
        width = SCREEN_WIDTH;
        height = SCREEN_HEIGHT;

        if (width % bucket_size != 0)
        {
            width = width - (width % bucket_size) + bucket_size;
        }

        if (height % bucket_size != 0)
        {
            height = height - (height % bucket_size) + bucket_size;
        }

        map_width = width;
        map_height = height;

        depth_layer_count = floor(height / tile_height) * 4;


        collision_map.max_load_factor(10000);
        collision_map.rehash( ((width * height) / (bucket_size * bucket_size)) );

        active_sprites.max_load_factor(10000);
        active_sprites.rehash(depth_layer_count);

        tile_map.rehash( ((width * height) / (bucket_size * bucket_size)) );

        active_tiles.max_load_factor(10000);
        active_tiles.rehash(depth_layer_count);
    }

    window_game->my_camera->mode = CAMERA_FOLLOW_PLAYER;

    window_game->my_camera->camera_view.x = 0;
    window_game->my_camera->camera_view.y = 0;

    if (room_id == ROOM_TITLE)
    {
        current_menu = new menu(MENU_TITLE,"","sprites/menus/spr_blue_grid.png","sprites/menus/spr_title.png","sprites/menus/spr_titlecard.png");
    }

    instance_create(obj_player,width/2 - 16,height/2 - 16,1);
}

room::~room()
{
    if (can_debug == 1)
    {
        delete main_debugger;
    }

    //Free all sprites

    for (int i = 0; i < SPR_TOTAL; i++)
    {
        if (sprite[i] != NULL)
        {
            delete sprite[i];
            sprite[i] = NULL;
        }
    }

    //Free all sounds

    for (int i = 0; i < SOUND_TOTAL; i++)
    {
        if (sound[i] != NULL)
        {
            Mix_FreeChunk( sound[i] );
            sound[i] = NULL;
        }
    }

    active_tiles.clear();
    tiles_dead.clear();
    tile_map.clear();

    global_id = 0;

    active_sprites.clear();
    objects_dead.clear();
    collision_map.clear();
    object_map.clear();

    obj_player.clear();
    obj_block.clear();

    player_pointer = NULL;

    my_background.clear();

    if (current_menu != NULL)
    {
        delete current_menu;
        current_menu = NULL;
    }

}

void room::draw()
{
    //printf("\nEnemies: %i",obj_enemy.size());

    //set camera_view variable based on whatever window the player is on

    if (player_pointer != NULL)
    {
        camera_view.x = player_pointer->my_window->my_camera->camera_view.x;
        camera_view.y = player_pointer->my_window->my_camera->camera_view.y;
        camera_view.w = player_pointer->my_window->my_camera->camera_view.w;
        camera_view.h = player_pointer->my_window->my_camera->camera_view.h;
    }


    //Activate tiles on screen

    for (auto i = window_game->my_camera->active_buckets.begin(); i != window_game->my_camera->active_buckets.end(); i++)
    {
        for (auto f = tile_map.begin(*i); f != tile_map.end(*i); f++)
        {
            f->second->activate();
        }
    }

    for (auto f = tile_map.begin(0); f != tile_map.end(0); f++)
    {
        f->second->activate();
    }

    //Draw background

    for (auto i = my_background.begin(); i != my_background.end(); i++)
    {
        i->second->draw();
    }


    //Draw active tiles and sprites

    for (int g = 0; g < depth_layer_count; g++)
    {
        for (auto i = active_tiles.begin(g); i != active_tiles.end(g); i++)
        {
            i->second->draw();
        }

        for (auto i = active_sprites.begin(g); i != active_sprites.end(g); i++)
        {
            i->second->basic_draw();
        }
    }

    //Pause Screen

    if ( game_paused == 1 )
    {

    }

    //Debug Stuff

    if (debug_mode == 1)
    {
        main_debugger->draw();
    }

    //HUD

    else
    {

    }


    //Draw Menu

    if (current_menu != NULL)
    {
        current_menu->draw();
    }
}

void room::step()
{
    if (room_alive == 1)
    {
        //camera stuff

        window_game->my_camera->step();

        if (debug_mode == 0)
        {
            if (game_paused == 0)
            {
                //Running object logic

                for (auto f = object_map.begin(); f != object_map.end(); f++)
                {
                    f->second->basic_step();

                    f->second->bucket_place();
                }

                //Pause

                if ( key_check_release(keyboard_pause) )
                {
                    game_paused = 1;
                }

                //Debug

                if ( key_check_release(keyboard_debug) )
                {
                    for (auto i = obj_block.begin(); i != obj_block.end(); i++)
                    {
                        i->second->visible = 1;
                    }

                    main_debugger->x = player_pointer->x;
                    main_debugger->y = player_pointer->y;

                    debug_mode = 1;
                }
            }
            else
            {
                //Unpause

                if ( key_check_release(keyboard_pause) )
                {
                    game_paused = 0;
                }
            }
        }
        else
        {
            //Run debugger logic

            main_debugger->step();

            //Debug

            if ( key_check_release(keyboard_debug) )
            {
                for (auto i = obj_block.begin(); i != obj_block.end(); i++)
                {
                    i->second->visible = 0;
                }

                debug_mode = 0;
            }
        }

        //deleting destroyed objects and tiles

        if (objects_dead.size() > 0)
        {

            for (auto i = objects_dead.begin(); i != objects_dead.end(); i ++)
            {
                int type_delete = -99;
                int thisone = -1;

                auto finder = collision_map.find( (*i).first );

                if ( finder != collision_map.end() )
                {
                    type_delete = (*i).second->object_type;
                    thisone = (*i).second->object_id;

                    collision_map.erase((*i).first);

                    object_map.erase(thisone);

                    if (type_delete == OBJ_PLAYER)
                    {
                        obj_player.erase(thisone);
                    }

                    if (type_delete == OBJ_BLOCK)
                    {
                        obj_block.erase(thisone);
                    }

                }

            }

            objects_dead.clear();

        }

        if (tiles_dead.size() > 0)
        {
            for (int i = 0; i < tiles_dead.size(); i++)
            {
                tile_map.erase(tiles_dead[i]);
            }

            tiles_dead.clear();
        }


        //Run Menu

        if (current_menu != NULL)
        {
            current_menu->step();
        }

    }
}


//debugger

debugger::debugger()
{
    collision_box.w = tile_width;
    collision_box.h = tile_height;
}

debugger::~debugger()
{

}

void debugger::step()
{
    //Snap debugger to grid

    x -= x % collision_box.w;
    y -= y % collision_box.h;

    collision_box.x = x;
    collision_box.y = y;

    player_pointer->x = x;
    player_pointer->y = y;


    //Change Player Start

    if (key_check_release(keyboard_primary))
    {
        player_pointer->xstart = x + (tile_width/2) - (player_pointer->collision_box.w/2);
        player_pointer->ystart = y + (tile_height/2) - (player_pointer->collision_box.h/2);
    }

    //Save Map

    if (key_check_release(keyboard_pause))
    {
        current_room->save();
    }

}

void debugger::draw()
{
    draw_sprite(sprite[SPR_PLAYER],player_pointer->xstart,player_pointer->ystart - 37,1,0.3);
}



#endif // CLASS_FUNCTIONS_H
