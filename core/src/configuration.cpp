//
// Created by george on 09.05.2026.
//

#include "../include/configuration.hpp"

utils::configuration::configuration(const configuration &other) :
    tape_path(other.tape_path), reading_delay(other.reading_delay),
    writing_delay(other.writing_delay), shift_delay(other.shift_delay),
    rewind_delay(other.reading_delay) {}

utils::configuration::configuration(configuration &&other) noexcept :
    tape_path(std::move(other.tape_path)), reading_delay(other.reading_delay),
    writing_delay(other.writing_delay), shift_delay(other.shift_delay),
    rewind_delay(other.reading_delay) {}

utils::configuration& utils::configuration::operator=(const configuration &other) {
    if (&other == this) return *this;
    configuration temp(other);
    swap(temp);
    return *this;
}

utils::configuration& utils::configuration::operator=(configuration &&other) noexcept {
    if (&other == this) return *this;
    configuration temp;
    swap(temp);
    swap(other);
    return *this;
}


utils::configuration::configuration(fs::path &&tape_path, fs::path &&configuration_path) :
    tape_path(std::move(tape_path)), reading_delay(0),
    writing_delay(0), shift_delay(0), rewind_delay(0) {
    using json = nlohmann::json;
    if (configuration_path.empty()) return;
    try {
        json config = json::parse(std::fstream(configuration_path));
        if (config.size() > 4)
            throw std::runtime_error("Too much fields in config.json");
        uint8_t fields = 0;
        for (auto it = config.cbegin(); it != config.cend(); ++it) {
            if (it.key() == "reading_delay") {
                reading_delay = it.value();
                fields++;
                continue;
            }
            if (it.key() == "writing_delay") {
                writing_delay = it.value();
                fields++;
                continue;
            }
            if (it.key() == "shift_delay") {
                shift_delay = it.value();
                fields++;
                continue;
            }
            if (it.key() == "rewind_delay") {
                rewind_delay = it.value();
                fields++;
            }
        }
        if (fields != 4)
            throw std::runtime_error("Incomplete config.json");
    } catch (const std::exception &exception) {
        throw configuration_error(exception.what());
    }

}

void utils::configuration::swap(configuration &other) noexcept {
    std::swap(tape_path, other.tape_path);
    std::swap(reading_delay, other.reading_delay);
    std::swap(writing_delay, other.writing_delay);
    std::swap(shift_delay, other.shift_delay);
    std::swap(rewind_delay, other.rewind_delay);
}

