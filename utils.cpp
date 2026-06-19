#include "utils.h"

#include <iostream>
#include <vector>
#include <map>
#include <filesystem>

namespace fs = std::filesystem;


void crash(std::string_view message) {
    std::cerr << "buildcon: " << message << std::endl;
    exit(EXIT_FAILURE);
}


int display_help(std::string_view command, std::string_view program_call) {
    std::map<std::string_view, std::string> help_router = {
        {"general", std::string("Usage: ") + std::string(program_call) + " <COMMAND> [OPTIONS] [ARGUMENTS]\n\n"
            "A C++ package manager and build system.\n\n"
            "Commands:\n"
            "  init                  Initialize a new project\n"
            "  install               Install libraries from remote to the local repository\n"
            "  compose               Build the current project\n"
            "  config                View or modify program settings\n"
            "  -v, --version         Display program version information\n"
            "  -h, --help            Show this help message\n\n"
            "Options are processed as letters following single or double dashes, e.g. -f, --f (these notations are equivalent). The available options depend on the command used. If omitted, default settings will be used.\n\n"
            "Arguments are mandatory and also depend on the command used.\n\n"
            "To learn more about a specific command, use " + std::string(program_call) + " <COMMAND> --help"
        },
        {"init", std::string("Usage: ") + std::string(program_call) + " [PROJECT_NAME]\n\n"
            "Command used to create a new project in the current working directory.\n"
            "The project name is mandatory and is the same as the name of the executable that your program will compile to."
        },
        {"install", std::string("Usage: ") + std::string(program_call) + " install [LIBS]\n\n"
        },
        {"compose", std::string("Usage: ") + std::string(program_call) + " compose\n\n"
        },
        {"config", std::string("Usage: ") + std::string(program_call) + " config [-g (global) -l (local) -d (display current config)] [SETTING VALUE]\n\n"
            "Command used to edit the project's configuration",
        },
        {"version", std::string("Usage: ") + std::string(program_call) + " version\n\n"
            "Displays the program's version"
        },
        {"help", "This is help"}
    };

    auto it = help_router.find(command);
    if(it == help_router.end()) {
        crash("attempted to access a non-existent help page");
    }

    std::cout << it->second << std::endl;
    return EXIT_SUCCESS;
}

std::optional<std::string> context_venv() {
    for(fs::path p=fs::current_path();p!="/";p=p.parent_path()) {
        if(fs::exists(p / ".bcenv")) {
            return p / ".bcenv";
        }
    }
    return std::nullopt;
}