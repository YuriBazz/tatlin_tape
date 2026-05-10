//
// Created by george on 09.05.2026.
//

#pragma once
#include <exception>
#include <string>

namespace utils {
    struct reading_error : std::exception {
        std::string info;
        explicit reading_error(const std::string &);
        const char* what() const noexcept override;
    };
    struct writing_error : std::exception {
        std::string info;
        explicit writing_error(const std::string &);
        const char* what() const noexcept override;
    };
    struct configuration_error : std::exception {
        std::string info;
        explicit configuration_error(const std::string &);
        const char* what() const noexcept override;
    };
}
