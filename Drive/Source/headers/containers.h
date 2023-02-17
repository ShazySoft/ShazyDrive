#ifndef CONTAINERS_H
#define CONTAINERS_H

///Storage

//Hash Functions

class spatial_hash
{
public:

    template<class S,class T,class U>

    size_t operator() (const pair<pair<S,T>,U> &coordinate_key) const
    {
        size_t this_hash = 0;

        double xcoord = coordinate_key.first.first;
        double ycoord = coordinate_key.first.second;

        if ((xcoord >= 0) & (xcoord <= map_width - bucket_size) & (ycoord >= 0) & (ycoord <= map_height - bucket_size))
        {
            this_hash = floor(xcoord / bucket_size) + 1 + (floor(ycoord / bucket_size) * (map_width / bucket_size));
        }
        else
        {
            this_hash = 0;
        }

        return this_hash;
    }

};

class equal_find
{
public:

    template<class S,class T,class U>

    bool operator() (const pair<pair<S,T>,U> &coordinate_key1,const pair<pair<S,T>,U> &coordinate_key2) const
    {
        return (coordinate_key1.second == coordinate_key2.second);
    }

};

class sprite_depth_hash
{
public:

    template<class S,class T>

    size_t operator() (const pair<S,T> &coordinate_key) const
    {
        size_t this_hash = 0;



        this_hash = coordinate_key.first;

        return this_hash;
    }

};

//Windows

vector<window*>windows;

//Hash Maps

unordered_map< pair<pair<double,double>,int>,object*,spatial_hash,equal_find>collision_map;
map<int,object*>object_map;

unordered_map< pair<int,int>, object*,sprite_depth_hash>active_sprites;

//Death Map

vector< pair<pair<pair<double,double>,int>,object*> > objects_dead;

//Specific Object Maps

map<int,unique_ptr<o_player>>obj_player;

map<int,unique_ptr<o_block>>obj_block;


//Tile Maps

unordered_map< pair<pair<double,double>,int>, unique_ptr<tile>, spatial_hash,equal_find>tile_map;

vector< pair<pair<double,double>,int> >tiles_dead;

unordered_map< pair<int,int>, tile*,sprite_depth_hash>active_tiles;


#endif // CONTAINERS_H
