#include "variables.h"

bool var_module_initialized = 0;

fs::path variable_file_path;

json::object variable_file_content;

void dir_creatation_error(const fs::path& attempted) {
    throw std::runtime_error(std::string("Error: unable to create directory ") + attempted.string() + " which is vital for the program's variable features");
}

bool ensure_dir(const fs::path& dir_path) {
    if(!fs::exists(dir_path)) {
        fs::create_directory(dir_path);
        if(!fs::exists(dir_path)) {
            dir_creatation_error(dir_path);
            return false;
        }
    }

    return true;
}

void create_var_file(const std::string& user_name) {
    fs::path current_step(std::string("/home/") + user_name + "/.config/");

    ensure_dir(current_step);

    current_step += "build_connector/";
    ensure_dir(current_step);

    current_step += "variables.json";
    std::ofstream actual_file(current_step);
    if(!fs::exists(current_step)) {
        throw std::runtime_error(std::string("Unable to create file ") + current_step.string());
    }
    actual_file << "{}";
    actual_file.close();
}

void init_var_module() {
    const char* username = std::getenv("USER");
    if(username == NULL) {
        throw std::runtime_error("Error: unable to fetch caller's username");
    }

    variable_file_path = std::string("/home/") + username + "/.config/build_connector/variables.json";
    if(!fs::exists(variable_file_path)) {
        create_var_file(username);
    }

    std::ifstream json_data(variable_file_path);
    if(!json_data.good()) {
        throw std::runtime_error(std::string("Error: unable to open file ") + variable_file_path.string());
    }
    
    try {
        variable_file_content = json::parse(json_data).as_object();
    }
    catch (...) {
        throw std::runtime_error(std::string("Error: variables file corrupted. Path to file: ") + variable_file_path.string());
    }

    var_module_initialized = 1;
}

void view_variables(bool as_cvs, bool with_headers) {
    if(!var_module_initialized) {
        init_var_module();
    }

    const char separator = as_cvs ? ',' : '\t';

    if(with_headers) {
        std::cout << "Alias" << separator << "Header file path" << separator << "Compiled library path\n";
    }

    for(const json::key_value_pair& i: variable_file_content) {
        std::cout << i.key() << separator << i.value().as_array().at(0).as_string().c_str() << separator << i.value().as_array().at(1).as_string().c_str() << '\n';
    }
}