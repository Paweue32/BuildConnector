#include "variables.h"

void dir_creatation_error(const fs::path& attempted) {
    crash(std::string("unable to create directory ") + attempted.string() + " which is vital for the program's aliasing features");
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
        crash(std::string("unable to create file ") + current_step.string());
    }
    actual_file << "{}";
    actual_file.close();
}

void init_var_module() {
    if(var_module_initialized) {
        return;
    }

    const char* username = std::getenv("USER");
    if(username == NULL) {
        crash("unable to fetch caller's username");
    }

    variable_file_path = std::string("/home/") + username + "/.config/build_connector/variables.json";
    if(!fs::exists(variable_file_path)) {
        create_var_file(username);
    }

    std::ifstream json_data(variable_file_path);
    if(!json_data.good()) {
        crash(std::string("unable to open file ") + variable_file_path.string());
    }
    
    try {
        variable_file_content = json::parse(json_data).as_object();
    }
    catch (...) {
        crash(std::string("alias file corrupted. Path to file: ") + variable_file_path.string());
    }

    var_module_initialized = 1;
}

void view_variables(bool as_csv, bool with_headers) {
    if(!var_module_initialized) {
        init_var_module();
    }

    const char separator = as_csv ? ',' : '\t';

    if(with_headers) {
        std::cout << "Alias" << separator << "Alias_type" << separator << "Alias_value\n";
    }

    for(const json::key_value_pair& i: variable_file_content) {
        std::cout << i.key() << separator << i.value().as_array().at(0).as_string().c_str() << separator << i.value().as_array().at(1).as_string().c_str() << '\n';
    }
}

void set_variable(const std::string& var_name, const library& content, bool overwrite_warning) {
    if(!var_module_initialized) {
        init_var_module();
    }

    auto json_iter = variable_file_content.find(var_name);
    if(json_iter == variable_file_content.end()) {
        variable_file_content[var_name] = json::array{content.variable_type, content.compiled_lib};
        return;
    }

    if(overwrite_warning) {
        std::cout << "Variable " << var_name << " already exists.\nType: " << json_iter->value().as_array().at(0).as_string() << "\nContent: " << json_iter->value().as_array().at(1).as_string() << "\nAre you sure you want to replace it (yes / no)? ";
        std::string ans;
        std::getline(std::cin, ans);
        while(ans != "yes" && ans != "no") {
            std::cout << "Not a viable response.\nDo you want to replace the variable (yes / no)? ";
            std::getline(std::cin, ans);
        }

        if(ans == "no") {
            return;
        }
    }

    variable_file_content[var_name] = json::array{content.variable_type, content.compiled_lib};
}

library read_variable(const std::string& var_name) {
    if(!var_module_initialized) {
        init_var_module();
    }

    auto json_iter = variable_file_content.find(var_name);
    if(json_iter == variable_file_content.end()) {
        return {};
    }
    return {json_iter->value().as_array().at(0).as_string().c_str(), json_iter->value().as_array().at(1).as_string().c_str()};
}

void delete_variable(const std::string& var_name) {
    if(!var_module_initialized) {
        init_var_module();
    }

    variable_file_content.erase(var_name);
}

void write_changes() {
    if(!var_module_initialized) {
        init_var_module();
    }

    std::ofstream json_data(variable_file_path);
    json_data << variable_file_content << std::endl;
    json_data.close();
}