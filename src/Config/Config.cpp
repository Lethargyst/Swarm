#include "Config.hpp"

Config& Config::Instance()
{
    static Config conf;

    std::ifstream file("../config/config.json");
    json data = json::parse(file);
    
    conf.threads_amount = data["threads_amount"];
    conf.max_fps = data["max_fps"];
    conf.max_period = 1.0 / conf.max_fps;
    conf.epsilon = data["epsilon"];
    
    conf.render_buffer_size = data["renderer"]["render_buffer_size"];
    conf.quad_tree_buffer_size = data["renderer"]["quad_tree_buffer_size"];
    conf.shout_lines_buffer_size = data["renderer"]["shout_lines_buffer_size"];

    conf.movement_spread = data["swarm"]["movement_spread"];
    conf.speed = data["swarm"]["speed"];
    conf.source_speed = data["swarm"]["source_speed"];
    conf.source_size = data["swarm"]["source_size"];
    conf.ant_size = data["swarm"]["ant_size"];
    conf.shout_range = data["swarm"]["shout_range"];
    conf.time_to_shout = data["swarm"]["time_to_shout"];
    conf.time_to_spread_dir = data["swarm"]["time_to_spread_dir"];

    conf.max_objects_per_node = data["quad_tree"]["max_objects_per_node"];
    conf.max_depth = data["quad_tree"]["max_depth"];

    conf.ants_amount= data["ants_amount"];
    conf.sources_amount = data["sources_amount"];

    return conf; 
}

Config& global = Config::Instance();