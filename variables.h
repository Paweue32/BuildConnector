#ifndef VARIABLES_H
#define VARIABLES_H

#include <filesystem>

namespace fs = std::filesystem;

struct library {
    std::string header_file;
    std::string compiled_lib;
};

fs::path variable_file;

bool create_var_file(const fs::path& file_path);
void init_var_module();
void view_variables();
void set_variable(const std::string& var_name, const std::string& header_file, const std::string& compiled_lib, bool overwrite_warning);
library read_variable(const std::string& var_name);
void delete_variable(const std::string& var_name);

#endif // VARIABLES_H