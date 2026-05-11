//
// Created by george on 10.05.2026.
//
#pragma once

#include <filesystem>
#include <fstream>

namespace utils {
/**
 * @brief Varp for temporary files. Has only default ctor.
 */
class tempfile {
    std::filesystem::path path_;
public:
    tempfile();
    ~tempfile();
    tempfile(const tempfile &) = delete;
    tempfile(tempfile &&) = delete;
    tempfile &operator=(const tempfile &) = delete;
    tempfile &operator=(tempfile &&) = delete;

    operator std::filesystem::path();
};
}
