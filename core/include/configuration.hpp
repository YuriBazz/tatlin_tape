//
// Created by george on 09.05.2026.
//
#pragma once
#include <string>
#include <fstream>
#include "../../libs/json.hpp"
#include "../include/exceptions.hpp"

namespace  utils {
    namespace fs =  std::filesystem;
/**
 * @brief Configuration object for a tape processor
 */
struct configuration {
    fs::path tape_path;
    size_t reading_delay;
    size_t writing_delay;
    size_t shift_delay;
    size_t rewind_delay;



    configuration(const configuration &);
    configuration(configuration &&) noexcept;
    configuration &operator=(const configuration &);
    configuration &operator=(configuration &&) noexcept;
    ~configuration() = default;


    /**
     *
     * @param tape_path Path to the tape file
     * @param configuration_path Path to the zeros.json file
     * @throws utils::configuration_error If something went wrong with the zeros.json file
     */
    explicit configuration(fs::path &&tape_path, fs::path &&configuration_path = "");
private:
    void swap(configuration &) noexcept;
    configuration() = default;
};

}

