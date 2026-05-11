//
// Created by george on 10.05.2026.
//

#include "tape_processer.hpp"

void utils::tape_processer::swap(tape_processer& other) noexcept {
    std::swap(tape_file_, other.tape_file_);
}


utils::tape_processer::tape_processer(const fs::path& tape_path) {
    if (!fs::exists(tape_path)) {
        // потому что по умолчанию оно вроде как не создается, что странно
        tape_file_.open(tape_path, std::ios::out);
        tape_file_.close();
    }
    tape_file_.open(tape_path, std::ios::in | std::ios::out | std::ios::binary);
    if (!tape_file_.is_open()) throw std::runtime_error("Could not open the file");
}

utils::tape_processer::tape_processer(tape_processer&& other) noexcept:
    tape_file_(std::move(other.tape_file_)) {}

utils::tape_processer& utils::tape_processer::operator=(tape_processer&& other) noexcept {
    if (&other == this) return *this;
    tape_processer temp;
    swap(temp);
    swap(other);
    return *this;
}

bool utils::tape_processer::read(int& dst) {
#ifdef SLEEPS
    std::this_thread::sleep_for(global_configuration.reading_delay);
#endif

    bool read(tape_file_.read(reinterpret_cast<char*>(&dst), sizeof(int)));
    bool e = eof();
    if (tape_file_.fail() && !eof())
        throw reading_error("Reading err");
    if (!e) tape_file_.seekg(-offset, std::ios_base::cur);
    return read;
}

bool utils::tape_processer::eof() const {
    return tape_file_.eof();
}

void utils::tape_processer::write(int value) {
#ifdef SLEEPS
    std::this_thread::sleep_for(global_configuration.writing_delay);
#endif
    tape_file_.write(reinterpret_cast<const char*>(&value), sizeof(int));
    if (tape_file_.fail())
        throw writing_error("Writing err");
    tape_file_.seekp(-offset, std::ios_base::cur);
}


void utils::tape_processer::rewind() {
#ifdef SLEEPS
    std::this_thread::sleep_for(global_configuration.reading_delay);
#endif
    tape_file_.clear();
    tape_file_.seekg(0);
    tape_file_.seekp(0);
}


//TODO: Подумать, что делать, если мы тут за пределы вышли.
void utils::tape_processer::shift_forward() {
#ifdef SLEEPS
    std::this_thread::sleep_for(global_configuration.shift_delay);
#endif
    tape_file_.seekp(offset, std::ios_base::cur);
    if (tape_file_.fail())
        throw std::runtime_error("IndexOutOfRangeException");
    //TODO: По-хорошему, тут бы сделать итератор
}

void utils::tape_processer::shift_backward() {
#ifdef SLEEPS
    std::this_thread::sleep_for(global_configuration.shift_delay);
#endif
    tape_file_.seekp(-offset, std::ios_base::cur);
    if (tape_file_.fail())
        throw std::runtime_error("IndexOutOfRangeException");
}
