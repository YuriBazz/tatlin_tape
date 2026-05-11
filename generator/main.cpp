//
// Created by george on 11.05.2026.
//

#include <cstring>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <filesystem>
#include <random>

int main(int argc, char *argv[]) {
    std::string src;
    size_t count = 10;

    static option long_options[] = {
        {"path", required_argument, 0, 'p'},
        {"count", required_argument, 0, 'c'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "p:c:", long_options, nullptr)) > -1) {
        switch (opt) {
        case 'p': src = optarg;
            break;
        case 'c': std::from_chars(optarg, optarg + std::strlen(optarg), count);
            break;
        default:
            std::cerr << "Undefined argument: " << optarg << std::endl;
            return 1;
        }
    }

    std::filesystem::path file(src);
    if (!std::filesystem::exists(file)) std::fstream creating(file, std::ios::out);
    std::fstream fs(file, std::ios::binary | std::ios::trunc | std::ios::in | std::ios::out);


    static std::mt19937_64 gen(time(NULL));
    std::uniform_int_distribution dist((int)-1e5, (int)1e5);

    for (size_t i = 0; i < count; ++i) {
        int x = dist(gen);
        fs.write(reinterpret_cast<char *>(&x), sizeof(int));
    }
}
