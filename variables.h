#ifndef VARIABLES_H
#define VARIABLES_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <boost/json.hpp>

namespace fs = std::filesystem;
namespace json = boost::json;

enum class variable_type {
    PATH,
    FLAG
};

struct library {
    variable_type kind;
    std::string compiled_lib;
};

inline bool var_module_initialized = 0;
inline fs::path variable_file_path;
inline json::object variable_file_content;

void create_var_file(const std::string& user_name);
void init_var_module();
const char* text(variable_type vt);
void view_variables(bool as_cvs, bool with_headers);
void set_variable(const std::string& var_name, const library& content, bool overwrite_warning);
library read_variable(const std::string& var_name);
void delete_variable(const std::string& var_name);
void write_changes();

#endif // VARIABLES_H