#ifndef PACKMAN_H
#define PACKMAN_H

#include <string>
#include <vector>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

int config(const std::vector<std::string_view>& args);
int init(const std::vector<std::string_view>& args);
int install(const std::vector<std::string_view>& args);


class PackageManager {
public:
    explicit PackageManager();
    PackageManager(const PackageManager& other) = delete;
    PackageManager(PackageManager&& other) = delete;
    ~PackageManager() = default;

    std::vector<std::string> installed_pkgs();
    fs::path user_dir() const { return user_dir_; }
    std::optional<fs::path> env_dir() const { return env_dir_; }

    PackageManager& operator=(const PackageManager& other) = delete;
    PackageManager& operator=(PackageManager&& other) = delete;
private:
    fs::path user_dir_;
    std::optional<fs::path> env_dir_;
};

#endif // PACKMAN_H