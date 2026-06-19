#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <string_view>
#include <optional>


void crash(std::string_view message);
int display_help(std::string_view command, std::string_view program_call);
std::optional<std::string> context_venv();

namespace constants {
    const inline std::string server_addr = "https://server.buildconnector.pl";
    const inline std::string version_info = "Buildconnector WIP";
} // namespace constants

#endif // UTILS_H