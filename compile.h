#ifndef COMPILE_H
#define COMPILE_H

#include <iostream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

inline bool verbose;
inline fs::path target;

/*bool compile(const std::string& file_base) {
    // Preprocessing
    system((std::string("g++ -E ") + file_base + ".cpp > " + file_base + ".ii || rm " + file_base + ".ii").c_str());
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
    std::cout << "Compiled " << file_base << ".cpp" << std::endl;

    system((std::string("rm ") + file_base + ".ii").c_str());
    return 1;
}

void link(const std::vector<std::string>& object_files) {
    std::string command = "g++ ";
    for(const std::string& objection: object_files) {
        command += objection + ' ';
    }
    command += std::string("-o ") + target.string();
    std::cout << command << std::endl;
    system(command.c_str());
}

void file_cleanup(const std::string& file_base) {
    fs::remove(fs::path(file_base + ".o"));
}*/


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