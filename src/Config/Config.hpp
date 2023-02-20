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

    unsigned threads_amount;
    float max_fps;
    float max_period;
    float epsilon;

    unsigned render_buffer_size;
    unsigned quad_tree_buffer_size;
    unsigned shout_lines_buffer_size;

    unsigned movement_spread;
    float speed;
    float source_speed;
    float source_size;
    float ant_size;
    float shout_range;

    unsigned time_to_shout;
    unsigned time_to_spread_dir;

    unsigned max_objects_per_node;
    unsigned max_depth;

    unsigned ants_amount;
    unsigned sources_amount;

private:
    Config() {}
    ~Config() {}
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
};

extern Config& global;

#endif