#ifndef COMPILE_H
#define COMPILE_H

#include <iostream>
#include <vector>
#include <filesystem>
#include <deque>
#include "variables.h"

namespace fs = std::filesystem;

//inline bool verbose;
//inline fs::path target;

int compose(const std::vector<std::string_view>& args);

bool compile(const std::string& file_base, bool verbose, bool save_temps);
void link(const std::vector<std::string>& object_files, const fs::path& target);
void file_cleanup(const std::string& file_base);
std::vector<std::string> get_files();
void do_your_thing(std::deque<std::pair<std::string, int>>& sources, std::deque<std::pair<std::string, int>>& vars, std::deque<std::pair<std::string, int>>& library_flags);
// Getting the files
    /*std::vector<std::string> files;
    for(const fs::__cxx11::directory_entry& dirEntry: fs::directory_iterator(fs::current_path())) {
        fs::path toHell(dirEntry);
        if(toHell.extension() == ".cpp") {
            std::string signature = toHell;
            files.push_back(signature.substr(0, signature.size() - 4));
        }
    }

    // Compilation
    for(size_t i=0;i<files.size();i++) {
        if(!compile(files[i])) {
            std::cout << "Error: could not compile " << files[i] << ".cpp into an object file." << std::endl;
            
            if(i == 0) {
                return EXIT_FAILURE;
            }

            std::string ans;
            while(ans != "y" && ans != "Y" && ans != "n" && ans != "N") {
                std::cout << "Do you want to destroy the already compiled object files? (y/n) ";
                std::zline(std::cin, ans);
            }
            if(ans == "n" || ans == "N") {
                return EXIT_FAILURE;
            }

            for(size_t j=0;j<i;j++) {
                file_cleanup(files[j]);
            }
            return EXIT_FAILURE;
        }
    }

    // Linking
    std::vector<std::string> objects;
    for(const std::string& prefix: files) {
        objects.push_back(prefix + ".o");
    }

    link(objects);
    
    std::cout << target.string() << '\t' << fs::exists(target) << std::endl;
    if(!fs::exists(target)) {
        std::cout << "Error: could not link the desired files into an executable." << std::endl;
        std::string ans;
        while(ans != "y" && ans != "Y" && ans != "n" && ans != "N") {
            std::cout << "Do you want to destroy the already compiled object files? (y/n) ";
            std::getline(std::cin, ans);
        }
        if(ans == "n" || ans == "N") {
            return EXIT_FAILURE;
        }

        for(const std::string& creation: files) {
            file_cleanup(creation);
        }
        return EXIT_FAILURE;
    }

    std::cout << "The building process was successful." << std::endl;
    std::string ans;
    while(ans != "y" && ans != "Y" && ans != "n" && ans != "N") {
        std::cout << "Do you want to destroy the already compiled object files? (y/n) ";
        std::getline(std::cin, ans);
    }
    if(ans == "n" || ans == "N") {
        return EXIT_SUCCESS;
    }

    for(const std::string& creation: files) {
        file_cleanup(creation);
    }*/

#endif // COMPILE_H