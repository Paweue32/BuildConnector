#include "parsing.h"

#include <format>
#include "utils.h"

std::pair<flags, std::vector<std::string_view>::const_iterator> parse_flags(std::vector<std::string_view>::const_iterator it, std::vector<std::string_view>::const_iterator end) {
    flags res;

    while(it != end && it->starts_with("-")) {
        if(it->size() == 1) {
            crash("expected a flag declaration after '-' symbol");
        }

        std::size_t i=1;
        if(it->operator[](1) == '-') {
            i = 2;
        }

        for(;i<it->size();i++) {
            if(it->operator[](i) < 'a' || it->operator[](i) > 'z') {
                crash(std::format("invalid flag '{}'", it->operator[](i)));
            }

            res[it->operator[](i)] = 1;
        }

        ++it;
    }

    return std::make_pair(std::move(res), std::move(it));
}

/*std::pair<flags, int> get_flags(const std::vector<std::string>& args, std::size_t start_index) { // returns the detected flags as well as the index of the first string that does not add new ones
    flags result;
    for(std::size_t i=start_index;i<args.size();i++) {
        if(args[i][0] != '-') {
            return std::make_pair(std::move(result), i);
        }

        if(args[i].size() == 1) {
            crash("expected a flag declaration after the '-' symbol");
        }

        for(std::size_t j=1;j<args[i].size();j++) {
            try {
                result[args[i][j]] = 1;
            }
            catch (...) {
                crash("flags may only be referenced with lowercase english characters");
            }
        }
    }

    return std::make_pair(std::move(result), args.size());
}

void assert_noflags(flags& leftovers) {
    for(char i='a';i<='z';i++) {
        if(leftovers[i]) {
            crash(std::string("flag ") + i + " is not supported by this command");
        }
    }
}

void assert_existance(const fs::path& tested) {
    if(!fs::exists(tested)) {
        crash(std::string("invalid path \"") + tested.c_str() + "\"");
    }
}

std::vector<std::string> unpack(const fs::path& starting_point, bool recursively, bool bottom_up) {
    ;
}


void parse_default(const std::vector<std::string>& args) {
    auto [exec_flags, index] = get_flags(args, 0);
    bool verbose = exec_flags['v'], save_temps = exec_flags['t'], recursive = exec_flags['r'], warn = exec_flags['w'], fully_static = exec_flags['s'], include_objects = exec_flags['o'], ensure_dynamic = exec_flags['d'];
    exec_flags['v'] = exec_flags['s'] = exec_flags['r'] = exec_flags['s'] = exec_flags['d'] = 0;
    assert_noflags(exec_flags);

    const std::set<std::string> supported_standards = {"98", "03", "11", "14", "17", "20", "23", "26"};
    std::string standard = "20";
    if(index < args.size() && args[index] == "std") {
        if(index + 1 == args.size()) {
            crash("a C++ standard must be specified after token \"std\"");
        }
        else if(!supported_standards.count(args[index+1])) {
            crash(args[index+1] + "is not among the supported standards (98, 03, 11, 14, 17, 20, 23, 26)");
        }

        standard = args[index+1];
        index += 2;
    }

    fs::path build_target;
    std::vector<std::string> sources;
    if(index == args.size()) {
        build_target = fs::current_path() / "a.out";
        sources.emplace_back(unpack(fs::current_path(), recursive));
    }
    else if(index + 1 == args.size()) {
        assert_existance(args[index]);
        
        if(fs::directory_entry(args[index]).is_directory()) {
            build_target = fs::path(args[index]) / "a.out";
            sources.emplace_back(unpack(args[index], recursive));
        }
        else {
            build_target = fs::path(args[index]).parent_path() / "a.out";
            sources.push_back(args[index]);
        }
    }
    else {
        if(fs::exists(args[index])) {
            if(fs::directory_entry(args[index]).is_directory()) {
                build_target = fs::path(args[index]) / "a.out";
            }
            else {
                build_target = args[index];
            }
        }
        else {
            if(fs::directory_entry(args[index]).is_directory()) {
                crash(std::string("invalid path \"") + args[index] + "\"");
            }
            else {
                assert_existance(fs::path(args[index]).parent_path());
                build_target = args[index];
            }
        }

        library skrot;
        for(std::size_t i=index+1;i<args.size();i++) {
            if(args[i][0] == '$') {
                skrot = read_variable(args[i].substr(1));
                if(skrot.compiled_lib == "") {
                    crash(std::string("alias ") + args[i].substr(1) + " does not exist");
                }
                if(skrot.variable_type == "Path") {
                    assert_existance(skrot.compiled_lib);
                }
            }
        }
    }
}

void parse_varset(const std::vector<std::string>& args) {
    auto [exec_flags, index] = get_flags(args, 2);
    bool as_flag = exec_flags['f'], warn = exec_flags['w'];
    exec_flags['f'] = exec_flags['w'] = 0;
    assert_noflags(exec_flags);

    if(index == args.size()) {
        crash("expected at least one alias to set");
    }

    if((args.size() - index) % 2 == 1) {
        crash("there must be a value provided for every alias");
    }

    for(std::size_t i=index;i<args.size();i+=2) {
        if(as_flag) {
            if(args[i+1][0] == '-') {
                set_variable(args[i], {"Flag", args[i+1]}, warn);
            }
            else {
                set_variable(args[i], {"Flag", std::string("-") + args[i+1]}, warn);
            }
        }
        else {
            if(args[i+1][0] == '-') {
                std::cout << "Variable " << args[i] << " not set: a path cannot begin with a '-' symbol" << std::endl;
            }
            else {
                set_variable(args[i], {"Path", args[i+1]}, warn);
            }
        }
    }

    write_changes();
}

void parse_varread(const std::vector<std::string>& args) {
    auto [exec_flags, index] = get_flags(args, 2);
    bool headings = exec_flags['h'], as_csv = exec_flags['c'], as_json = exec_flags['j'];
    exec_flags['h'] = exec_flags['c'] = exec_flags['j'] = 0;
    assert_noflags(exec_flags);

    if(as_csv && as_json) {
        crash("flags 'c' and 'j' contradict each other and cannot both be used");
    }

    if(index == args.size()) {
        crash("expected at least one alias to read");
    }

    library biblioteka;
    for(std::size_t i=index;i<args.size();i++) {
        biblioteka = read_variable(args[i]);
        if(biblioteka.compiled_lib == "") {
            crash(std::string("alias ") + args[i] + " does not exist");
        }

        if(as_json) {
            if(headings && i == index) {
                std::cout << json::object( {
                    {"Alias", json::array( { "Alias_type", "Alias_value" } )}
                } ) << std::endl;
            }

            std::cout << json::object( {
                {args[i], json::array( { biblioteka.variable_type, biblioteka.compiled_lib } )}
            } ) << std::endl;
        }
        else if(as_csv) {
            if(headings && i == index) {
                std::cout << "Alias,Alias_type,Alias_value" << std::endl;
            }

            std::cout << args[i] << ',' << biblioteka.variable_type << ',' << biblioteka.compiled_lib << std::endl;
        }
        else {
            if(headings && i == index) {
                std::cout << "Alias\tAlias_type\tAlias_value" << std::endl;
            }

            std::cout << args[i] << '\t' << biblioteka.variable_type << '\t' << biblioteka.compiled_lib << std::endl;
        }
    }
}

void parse_varview(const std::vector<std::string>& args) {
    auto [exec_flags, index] = get_flags(args, 2);
    bool headings = 1 - exec_flags['n'], as_csv = exec_flags['c'], as_json = exec_flags['j'];
    exec_flags['n'] = exec_flags['c'] = exec_flags['j'] = 0;
    assert_noflags(exec_flags);

    if(as_csv && as_json) {
        crash("flags 'c' and 'j' contradict each other and cannot both be used");
    }

    if(index != args.size()) {
        crash(std::string("unexpected token \"") + args[index] + "\"");
    }

    if(as_csv) {
        view_variables(export_format::CSV, headings);
    }
    else if(as_json) {
        view_variables(export_format::JSON, headings);
    }
    else {
        view_variables(export_format::HUMAN, headings);
    }
}

void parse_vardelete(const std::vector<std::string>& args) {
    if(args.size() == 2) {
        crash("no alias provided");
    }

    auto [exec_flags, index] = get_flags(args, 2);
    assert_noflags(exec_flags);
    
    for(size_t i=2;i<args.size();i++) {
        delete_variable(args[i]);
    }
    write_changes();
}*/