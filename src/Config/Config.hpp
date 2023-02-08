#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <iostream>
#include "Json/json.hpp" 

using json = nlohmann::json;

class Config 
{
public:
    static Config& Instance();

    float max_fps;
    float max_period;
    float epsilon;

    unsigned int render_buffer_size;
    unsigned int quad_tree_buffer_size;
    unsigned int shout_lines_buffer_size;

    unsigned int movement_spread;
    float speed;
    float source_speed;
    float source_size;
    float ant_size;
    float shout_range;

    unsigned int time_to_shout;
    unsigned int time_to_spread_dir;

    unsigned int max_objects_per_node;
    unsigned int max_depth;

    unsigned int ants_amount;
    unsigned int sources_amount;

private:
    Config() {}
    ~Config() {}
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

extern Config& global;

#endif