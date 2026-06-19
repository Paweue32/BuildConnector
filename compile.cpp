#include <queue>
#include "compile.h"


int compose(const std::vector<std::string_view>& args) { return EXIT_SUCCESS; }


bool compile(const std::string& file_base, bool verbose, bool save_temps) {
    // Preprocessing
    system((std::string("g++ -E ") + file_base + ".cpp > " + file_base + ".ii").c_str());
    if(!fs::exists(fs::path(file_base + ".ii"))) {
        return 0;
    }
    if(verbose) {
        std::cout << "Preprocessed " << file_base << ".cpp" << std::endl;
    }
    
    // Actually compiling
    if(verbose) {
        system((std::string("g++ -c ") + file_base + ".ii -o " + file_base + ".o -Wall").c_str());
    }
    else {
        system((std::string("g++ -c ") + file_base + ".ii -o " + file_base + ".o").c_str());
    }

    if(!fs::exists(fs::path(file_base + ".o"))) {
        system((std::string("rm ") + file_base + ".ii").c_str());
        return 0;
    }

    if(verbose) {
        std::cout << "Compiled " << file_base << ".cpp" << std::endl;
    }
    if(!save_temps) {
        system((std::string("rm ") + file_base + ".ii").c_str());
    }
    return 1;
}

void link(const std::vector<std::string>& object_files, const fs::path& target) {
    std::string command = "g++ ";
    for(const std::string& objection: object_files) {
        command += objection + ' ';
    }
    command += std::string("-o ") + target.string();
    std::cout << command << std::endl;
    system(command.c_str());
}

struct comparator {
    bool operator() (const std::pair<int,int>& a, const std::pair<int,int>& b) {
        return a.second < b.second;
    }
};

void file_cleanup(const std::string& file_base) {
    fs::remove(fs::path(file_base + ".o"));
}

void do_your_thing(std::deque<std::pair<std::string, int>>& sources, std::deque<std::pair<std::string, int>>& vars, std::deque<std::pair<std::string, int>>& library_flags) {
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, comparator> action;

    if(sources.size()) {
        action.push({0, sources[0].second});
    }
    if(vars.size()) {
        action.push({1, vars[0].second});
    }
    if(library_flags.size()) {
        action.push({2, library_flags[0].second});
    }

    while(!action.empty()) {
        std::pair<int,int> current = action.top();
        action.pop();

        if(current.first == 0) {
            std::cout << "source: " << sources[0].first << std::endl;
            sources.erase(sources.begin());
            if(sources.size()) {
                action.push({0, sources[0].second});
            }
        }
        else if(current.first == 1) {
            std::cout << "var: " << vars[0].first << std::endl;
            vars.erase(vars.begin());
            if(vars.size()) {
                action.push({1, vars[0].second});
            }
        }
        else {
            std::cout << "library flag: " << library_flags[0].first << std::endl;
            library_flags.erase(library_flags.begin());
            if(library_flags.size()) {
                action.push({2, library_flags[0].second});
            }
        }
    }
}