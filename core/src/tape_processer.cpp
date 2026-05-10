//
// Created by george on 10.05.2026.
//

#include "../include/tape_processer.hpp"
void utils::tape_processer::swap(tape_processer& other) noexcept {
    std::swap(tape_file_, other.tape_file_);
    std::swap(reading_delay_, other.reading_delay_);
    std::swap(writing_delay_, other.writing_delay_);
    std::swap(shift_delay_, other.shift_delay_);
    std::swap(rewind_delay_, other.rewind_delay_);
}

utils::tape_processer::tape_processer(const configuration& configuration):
    reading_delay_(configuration.reading_delay),
    writing_delay_(configuration.writing_delay), shift_delay_(configuration.shift_delay),
    rewind_delay_(configuration.rewind_delay) {
    if (!fs::exists(configuration.tape_path)) {
        std::fstream creating(configuration.tape_path, std::ios::out); // потому что по умолчанию оно вроде как не создается, что странно
    }
    tape_file_ = std::fstream(configuration.tape_path, std::ios::in | std::ios::out | std::ios::binary);
}

utils::tape_processer::tape_processer(tape_processer&& other) noexcept:
    tape_file_(std::move(other.tape_file_)), reading_delay_(other.reading_delay_),
    writing_delay_(other.writing_delay_), shift_delay_(other.shift_delay_),
    rewind_delay_(other.rewind_delay_) {}

utils::tape_processer& utils::tape_processer::operator=(tape_processer&& other) noexcept {
    if (&other == this) return *this;
    tape_processer temp;
    swap(temp);
    swap(other);
    return *this;
}

bool utils::tape_processer::read(int& dst) {
#ifdef SLEEPS
    std::this_thread::sleep_for(reading_delay_);
#endif

    bool read(tape_file_.read(reinterpret_cast<char*>(&dst), sizeof(int)));
    bool e = eof();
    if (tape_file_.fail() && !eof())
        throw reading_error("Reading err");
    if (!e) tape_file_.seekg(-sizeof(int), std::ios_base::cur);
    return read;
}

bool utils::tape_processer::eof() const {
    return tape_file_.eof();
}

void utils::tape_processer::write(int value) {
#ifdef SLEEPS
    std::this_thread::sleep_for(writing_delay_);
#endif
    try {
        tape_file_.write(reinterpret_cast<const char*>(&value), sizeof(int));
    }catch (const std::exception &ex) {
        throw writing_error(ex.what());
    }
    tape_file_.seekp(-sizeof(int), std::ios_base::cur);
}


void utils::tape_processer::rewind() {
#ifdef SLEEPS
    std::this_thread::sleep_for(rewind_delay_);
#endif
    tape_file_.clear();
    tape_file_.seekg(0);
    tape_file_.seekp(0);
}


//TODO: Подумать, что делать, если мы тут за пределы вышли.
void utils::tape_processer::shift_forward() {
#ifdef SLEEPS
    std::this_thread::sleep_for(shift_delay_);
#endif
    tape_file_.seekp(sizeof(int), std::ios_base::cur);
    //TODO: По-хорошему, тут бы сделать итератор
}

void utils::tape_processer::shift_backward() {
#ifdef SLEEPS
    std::this_thread::sleep_for(shift_delay_);
#endif
    tape_file_.seekp(-sizeof(int), std::ios_base::cur);
    if (tape_file_.fail())
        throw std::runtime_error("ну вышел за пределы файлика.");
}
