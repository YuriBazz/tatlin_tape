//
// Created by george on 11.05.2026.
//
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <filesystem>
#include <random>
#include <vector>

int main(int argc, char *argv[]) {
    std::string src, dst;

    static option long_options[] = {
        {"orig", required_argument, 0, 'o'},
        {"sort", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "o:s:", long_options, nullptr)) > -1) {
        switch (opt) {
        case 'o': src = optarg;
            break;
        case 's': dst = optarg;
            break;
        default:
            std::cerr << "Undefined argument: " << optarg << std::endl;
            return 1;
        }
    }

    std::fstream original(src, std::ios::binary | std::ios::out | std::ios::in);
    std::fstream sorted(dst, std::ios::binary | std::ios::out | std::ios::in);

    std::vector<int> v;
    int val;
    while (original.read(reinterpret_cast<char*>(&val), sizeof(int))) {
        v.push_back(val);
    }
    std::sort(v.begin(), v.end());

    std::vector<int> res;

    while (sorted.read(reinterpret_cast<char *>(&val), sizeof(int))) {
        res.push_back(val);
    }

    if (res.size() != v.size()) {
        std::cout << 0 << std::endl;
        return 0;
    }

    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != res[i]) {
            std::cout << 0 << std::endl;
            return 0;
        }
    }
    std::cout << 1 << std::endl;
}
