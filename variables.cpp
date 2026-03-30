#include "variables.h"

bool var_module_initialized = 0;

fs::path variable_file_path = "/home/paweue/.config/build_connector/variables.json";

json::object variable_file_content = {
    {"GLFW", json::array{"/usr/include/GLFW.h", "/usr/lib/GLFW.a"}},
    {"BOOST_JSON", json::array{"/usr/include/boost/json.hpp", "/usr/lib/boost/json.so"}}
};

void init_var_module() {
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