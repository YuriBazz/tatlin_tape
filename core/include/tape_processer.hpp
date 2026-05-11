//
// Created by george on 09.05.2026.
//
#pragma once
#include <fstream>
#include "exceptions.hpp"
#include "configuration.hpp"
#include "sorter.hpp"
#include <thread>

namespace utils {


/**
 * @brief Processor for the tape files.
 * @note Warp above the std::fstream
 */
class tape_processer {
    std::fstream tape_file_;

    tape_processer() = default;

    void swap(tape_processer &other) noexcept;
    static constexpr std::streamoff offset = sizeof(int);


public:
    tape_processer(const fs::path& tape_path);

    tape_processer(const tape_processer &) = delete;
    /**
     * @brief Move ctor for a tape processor.
     * @param other Tape processor to be moved
     */
    tape_processer(tape_processer&& other) noexcept;


    tape_processer &operator=(const tape_processer &) = delete;

    /**
     * @brief Assignment operator.
     * @param other r-value reference to the other tape processor
     * @return l-value reference to the result of an assignment
     */
    tape_processer& operator=(tape_processer&& other) noexcept;

    ~tape_processer() = default;


    /**
     * @brief Reads value into dst
     * @param dst Destination to read value
     * @return true if value was read, false -- otherwise
     * @throws utils::reading_error If something went wrong while reading the tape
     */
    bool read(int &dst);

    /**
     * @brief Verify that the current cursor's position is the end of the tape
     * @return true if the cursor points to the eof of the tape, false -- otherwise
     */
    bool eof() const;

    /**
     * @brief Write int value to the tape file
     * @param value value to be written to the tape
     * @throws utils::writing_error If something went wrong
     */
    void write(int value);

    /**
     * @brief Rewind the cursor's position
     */
    void rewind();


    /**
     * @brief Shifts cursor's position forward
     * @throws std::runtime_error If tried to shift outside the file
     */
    void shift_forward();
    /**
     * @brief Shifts cursor's position backward
     * @throws std::runtime_error If tried to shift outside the file
     */
    void shift_backward();

};

}
