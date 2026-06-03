#include "utils.h"

#include <iostream>
#include <map>
#include <format>


void crash(std::string_view message) {
    std::cerr << "buildcon: " << message << std::endl;
    exit(EXIT_FAILURE);
}


int display_help(std::string_view command, std::string_view program_call) {
    std::map<std::string_view, std::string> help_router = {
        {"general", std::format("Usage: {} <COMMAND> [OPTIONS] [ARGUMENTS]\n\n"
            "A C++ package manager and build system.\n\n"
            "Commands:\n"
            "  init                  Initialize a new project\n"
            "  install               Install libraries from remote to the local repository\n"
            "  include               Add locally installed libraries to the current project\n"
            "  compose               Build the current project\n"
            "  config                View or modify program settings\n"
            "  -v, --version         Display program version information\n"
            "  -h, --help            Show this help message\n\n"
            "Options are processed as letters following single or double dashes, e.g. -f, --f (these notations are equivalent). The available options depend on the command used. If omitted, default settings will be used.\n\n"
            "Arguments are mandatory and also depend on the command used.\n\n"
            "To learn more about a specific command, use {} <COMMAND> --help",
            program_call, program_call
        )}
    };

    auto it = help_router.find(command);
    if(it == help_router.end()) {
        crash("attempted to access a non-existent help page");
    }

    std::cout << it->second << std::endl;
    return EXIT_SUCCESS;
}