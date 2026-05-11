//
// Created by george on 09.05.2026.
//
#include "exceptions.hpp"
utils::configuration_error::configuration_error(const std::string &what) :info(what)  {}

const char* utils::configuration_error::what() const noexcept {
    return info.c_str();
}

utils::reading_error::reading_error(const std::string &info) : info(info) {}

const char* utils::reading_error::what() const noexcept {
    return info.c_str();
}

utils::writing_error::writing_error(const std::string &info) :info(info) {}

const char* utils::writing_error::what() const noexcept {
    return info.c_str();
}
