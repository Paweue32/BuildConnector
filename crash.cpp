#include "crash.h"

void crash(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
    exit(EXIT_FAILURE);
}