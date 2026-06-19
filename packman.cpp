#include "packman.h"

#include <iostream>
#include <fstream>
#include <boost/json.hpp>
#include "parsing.h"
#include "utils.h"
#include "network_client.h"

fs::path load_user_path();


int init(const std::vector<std::string_view>& args) {
    if(args.size() > 2 && args[2] == "--help") {
        return display_help("init", args[0]);
    }

    auto [init_flags, it] = parse_flags(args.begin() + 2, args.end());
    bool force = init_flags.extract('f');
    init_flags.assert_empty();

    fs::path env_path;
    if(it == args.end()) {
        env_path = fs::current_path() / ".bcenv";
    }
    else if(it + 1 == args.end()) {
        env_path = *it;
        if(!fs::exists(env_path)) {
            crash(env_path.string() + " does not exist");
        }
        env_path /= "bcproject";
    }
    else {
        crash("too many arguments provided for the init command");
    }

    if(fs::exists(env_path)) {
        if(force) {
            fs::remove_all(env_path);
        }
        else {
            crash(env_path.parent_path().string() + " is already a buildconnector project");
        }
    }

    try {
        if(!fs::create_directory(env_path)) {
            throw 1;
        }
        if(!fs::create_directory(env_path / "include")) {
            throw 1;
        }
        if(!fs::create_directory(env_path / "lib")) {
            throw 1;
        }
        if(!fs::create_directory(env_path / "pkgs")) {
            throw 1;
        }

        std::ofstream config_file = env_path / "config.json";
        if(!fs::exists(env_path / "config.json") || !config_file.good()) {
            throw 1;
        }
        config_file.close();

        config_file.open(env_path.parent_path() / "BCproject.txt");
        if(!fs::exists(env_path.parent_path() / "BCproject.txt") || !config_file.good()) {
            throw 1;
        }
    }
    catch(...) {
        crash("failed to create the initialize a buildconnector project in desired dir");
    }

    return EXIT_SUCCESS;
}

int install(const std::vector<std::string_view>& args) {
    if(args.size() > 2 && args[2] == "--help") {
        std::cout << "I'm helping rn" << std::endl;
        return EXIT_SUCCESS;
    }

    auto [install_flags, it] = parse_flags(args.begin() + 2, args.end());
    bool reinstall = install_flags.extract('r'), local = install_flags.extract('l'), download_only = install_flags.extract('d');
    install_flags.assert_empty();

    std::vector<std::string> to_install = {"5"};
    // ...

    if(to_install.empty()) {
        std::cout << "Requested libs already installed" << std::endl;
        return EXIT_SUCCESS;
    }

    PackageManager packman;
    std::vector<std::string> to_download = {"wt", "boost", "gmock"};
    // ...

    if(to_download.size()) {
        NetworkClient client(std::nullopt);
        std::vector<std::string> actual_downloads = client.POST_scout(to_download);
        std::cout << "Libs to install: [ ";
        std::copy(actual_downloads.begin(), actual_downloads.end(), std::ostream_iterator<std::string>(std::cout, " "));
        std::cout << "]" << std::endl;

        std::vector<std::string> failures = client.GET_download(actual_downloads, packman.user_dir() / "pkgs");
        if(failures.size()) {
            std::cerr << "Failed to download libs: [ ";
            std::copy(failures.begin(), failures.end(), std::ostream_iterator<std::string>(std::cerr, " "));
            std::cerr << "]" << std::endl;

            for(const std::string& failure: failures) {
                fs::remove(packman.user_dir() / "pkgs" / (failure + ".tar.gz"));
            }

            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

int config(const std::vector<std::string_view>& args) {
    if(args.size() > 2 && args[2] == "--help") {
        std::cout << "Hellpig" << std::endl;
        return EXIT_SUCCESS;
    }

    auto [config_flags, it] = parse_flags(args.begin() + 2, args.end());
    bool global = config_flags.extract('g'), local = config_flags.extract('l'), display_current = config_flags.extract('d');
    config_flags.assert_empty();


    PackageManager packman;
    if(display_current) {
        if(!global && !local) {
            global = true;
            if(packman.env_dir().has_value()) {
                local = true;
            }
        }
        else if(local && !packman.env_dir().has_value()){
            crash("cannot display local config without a project context");
        }

        if(global) {
            try {
                std::ifstream gfile = packman.user_dir() / "config.json";
                boost::json::object settings = boost::json::parse(gfile).as_object();
            }
            catch(...) {
                crash("failed to load user's global config");
            }
        }
        return EXIT_SUCCESS;
    }

    if(global && local) {
        crash("outside of display, flags 'g' and 'l' contradict each other");
    }

    return EXIT_SUCCESS;
}

PackageManager::PackageManager() : user_dir_(load_user_path()), env_dir_(context_venv()) {}

fs::path load_user_path() {
    fs::path user_path = fs::path(std::string("/home/") + getenv("USER") + "/.buildcon");
    if(fs::exists(user_path)) {
        return user_path;
    }

    try {
        if(!fs::create_directory(user_path)) {
            throw 1;
        }
        if(!fs::create_directory(user_path / "include")) {
            throw 1;
        }
        if(!fs::create_directory(user_path / "lib")) {
            throw 1;
        }
        if(!fs::create_directory(user_path / "pkgs")) {
            throw 1;
        }

        std::ofstream config_file = user_path / "config.json";
        if(!fs::exists(user_path / "config.json") || !config_file.good()) {
            throw 1;
        }
        config_file.close();
    }
    catch(...) {
        crash("failed to create user-level directories");
    }

    return user_path;
}