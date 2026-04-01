#ifndef PARSING_H
#define PARSING_H

#include "variables.h"
#include "compile.h"
#include "variables.h"

void parse_default(const std::vector<std::string>& args);
void parse_varset(const std::vector<std::string>& args);
void parse_varread(const std::vector<std::string>& args);
void parse_varview(const std::vector<std::string>& args);
void parse_vardelete(const std::vector<std::string>& args);

#endif // PARSING_H