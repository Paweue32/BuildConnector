#ifndef PARSING_H
#define PARSING_H

#include <vector>
#include <string_view>
#include <assert.h>
#include <format>
#include "utils.h"

struct flags {
    std::vector<bool> used = std::vector<bool>(26, 0);

    bool extract(char index) {
        bool res = operator[](index);
        operator[](index) = 0;
        return res;
    }

    void assert_empty() {
        for(std::size_t i=0;i<26;i++) {
            if(used[i]) {
                crash(std::format("invalid flag '{}'", static_cast<char>('a' + i)));
            }
        }
    }

    std::_Bit_reference operator[](char index) {
        return used[index-'a'];
    }
};

std::pair<flags, std::vector<std::string_view>::const_iterator> parse_flags(std::vector<std::string_view>::const_iterator it, std::vector<std::string_view>::const_iterator end);

#endif // PARSING_H