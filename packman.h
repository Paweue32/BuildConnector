#ifndef PACKMAN_H
#define PACKMAN_H

#include <string>
#include <vector>
#include <optional>


int install(const std::vector<std::string_view>& args);


class PackageManager {
public:
    explicit PackageManager(const std::optional<std::string>& custom_pkg_dir);
    PackageManager(const PackageManager& other) = delete;
    PackageManager(PackageManager&& other) = delete;
    ~PackageManager() = default;

    std::vector<std::string> installed_pkgs();

    PackageManager& operator=(const PackageManager& other) = delete;
    PackageManager& operator=(PackageManager&& other) = delete;
private:
    std::string pkg_dir_;
};

#endif // PACKMAN_H