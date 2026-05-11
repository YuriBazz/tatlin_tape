//
// Created by george on 09.05.2026.
//

#include "../include/configuration.hpp"

utils::configuration utils::global_configuration;

utils::configuration::configuration(fs::path &&configuration_path) {
    using json = nlohmann::json;
    if (configuration_path.empty()) return;
    try {
        json config = json::parse(std::fstream(configuration_path));
        if (config.size() > 4)
            throw std::runtime_error("Too much fields in config.json");
        uint8_t fields = 0;
        for (auto it = config.cbegin(); it != config.cend(); ++it) {
            if (it.key() == "reading_delay") {
                reading_delay = std::chrono::microseconds(it.value());
                fields++;
                continue;
            }
            if (it.key() == "writing_delay") {
                writing_delay = std::chrono::microseconds(it.value());
                fields++;
                continue;
            }
            if (it.key() == "shift_delay") {
                shift_delay = std::chrono::microseconds(it.value());
                fields++;
                continue;
            }
            if (it.key() == "rewind_delay") {
                rewind_delay = std::chrono::microseconds(it.value());
                fields++;
            }
        }
        if (fields != 4)
            throw std::runtime_error("Incomplete config.json");
    } catch (const std::exception &exception) {
        throw configuration_error(exception.what());
    }

}