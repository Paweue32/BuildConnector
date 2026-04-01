#include "parsing.h"

// Parsing arguments
/*std::set<std::string> args;
std::vector<std::string> variables;
for(int i=1;i<argc;i++) {
    args.insert(std::string(argv[i]));
}
for(const std::string& info: args) {
    if(info == "-v") {
        verbose = 1;
    }
    else if(info[0] == '$') {
        variables.push_back(info);
    }
}*/

void parse_default(const std::vector<std::string>& args) {
    ;
}

void parse_varset(const std::vector<std::string>& args) {
    ;
}

void parse_varread(const std::vector<std::string>& args) {
    library result;
    bool as_csv = 0;

    for(size_t i=2;i<args.size();i++) {
        if(args[i][0] == '-') {
            for(size_t j=1;j<args[i].size();j++) {
                if(args[i][j] == 'c') {
                    as_csv = 1;
                }
                else {
                    crash(std::string("flag -") + args[i][j] + " not supported in the \"alias table\" script");
                }
            }
        }
        else {
            result = read_variable(args[i]);
            char separator = as_csv ? ',' : '\t';
            if(result.compiled_lib.size()) {
                std::cout << args[i] << separator << result.variable_type << separator << result.compiled_lib << std::endl;
            }
            exit(EXIT_SUCCESS);
        }
    }

    crash("no alias provided");
}

void parse_varview(const std::vector<std::string>& args) {
    bool as_csv = 0, headings = 1;

    for(size_t i=2;i<args.size();i++) {
        if(args[i][0] != '-') {
            crash(std::string("unexpected token \"") + args[i] + '"');
        }
        for(size_t j=1;j<args[i].size();j++) {
            if(args[i][j] == 'c') {
                as_csv = 1;
            }
            else if(args[i][j] == 'n') {
                headings = 0;
            }
            else {
                crash(std::string("flag -") + args[i][j] + " not supported in the \"alias table\" script");
            }
        }
    }

    view_variables(as_csv, headings);
}

void parse_vardelete(const std::vector<std::string>& args) {
    if(args.size() == 2) {
        crash("no alias provided");
    }
    
    for(size_t i=2;i<args.size();i++) {
        delete_variable(args[i]);
    }
    write_changes();
}