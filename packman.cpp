#include "packman.h"

#include <iostream>
#include <filesystem>
#include "parsing.h"

namespace fs = std::filesystem;
std::string find_pkg_dir(const std::optional<std::string>& custom_pkg_dir);


int install(const std::vector<std::string_view>& args) {
    if(args.size() > 2 && args[2] == "--help") {
        std::cout << "I'm helping rn" << std::endl;
        return EXIT_SUCCESS;
    }

    auto [install_flags, it] = parse_flags(args.begin() + 2, args.end());

    bool reinstall = install_flags.extract('r');
    install_flags.assert_empty();

    return EXIT_SUCCESS;
}



PackageManager::PackageManager(const std::optional<std::string>& custom_pkg_dir) : pkg_dir_(find_pkg_dir(custom_pkg_dir)) {
    ;
}

std::string find_pkg_dir(const std::optional<std::string>& custom_pkg_dir) {
    if(custom_pkg_dir.has_value()) {
        ;
    }

    return "";
}