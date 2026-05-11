//
// Created by george on 09.05.2026.
//
#pragma once
#include "json.hpp"
#include "exceptions.hpp"
#include <chrono>
#include <fstream>

namespace  utils {
    namespace fs =  std::filesystem;
/**
 * @brief Configuration object for a tape processor
 */
struct configuration {
    std::chrono::microseconds reading_delay{0};
    std::chrono::microseconds writing_delay{0};
    std::chrono::microseconds shift_delay{0};
    std::chrono::microseconds rewind_delay{0};

    /**
     *
     * @param configuration_path Path to the zeros.json file
     * @throws utils::configuration_error If something went wrong with the config.json file
     */
    configuration(fs::path &&configuration_path);

    configuration() = default;
    ~configuration() = default;
    configuration(const configuration &) = default;
    configuration(configuration &&) = default;
    configuration &operator=(const configuration &) = default;
    configuration &operator=(configuration &&) = default;
};

extern configuration global_configuration;
}
