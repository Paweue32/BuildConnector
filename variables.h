#ifndef VARIABLES_H
#define VARIABLES_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <boost/json.hpp>

namespace fs = std::filesystem;
namespace json = boost::json;

struct library {
    std::string header_file;
    std::string compiled_lib;
};

extern bool var_module_initialized;
extern fs::path variable_file_path;
extern json::object variable_file_content;

extern void create_var_file(const std::string& user_name);
extern void init_var_module();
extern void view_variables(bool as_cvs, bool with_headers);
extern void set_variable(const std::string& var_name, const std::string& header_file, const std::string& compiled_lib, bool overwrite_warning);
extern library read_variable(const std::string& var_name);
extern void delete_variable(const std::string& var_name);
extern void write_changes();

#endif // VARIABLES_H