#include <getopt.h>
#include <iostream>
#include "include/all.hpp"

// 1 always for current bin
// --src=    --dst=     --config=     --ram_size=
// 5
int main(int argc, char *argv[]) {
    size_t ram_size = 0;
    std::string src, dst, config;

    static option long_options[] = {
        {"src", required_argument, 0, 's'},
        {"dst", required_argument, 0, 'd'},
        {"config", required_argument, 0, 'c'},
        {"ram", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "s:d:c:r:", long_options, nullptr)) > -1) {
        switch (opt) {
        case 's': src = optarg;
            break;
        case 'd': dst = optarg;
            break;
        case 'c' : config = optarg;
            break;
        case 'r' : std::from_chars(optarg, optarg + std::strlen(optarg), ram_size);
            break;
        default:
            std::cerr << "Undefined argument: " << optarg << std::endl;
            return 1;
        }
    }

    if (src.empty() || dst.empty()) {
        std::cerr << "Must specify the path to source and to destination: --src=<path>  --dst=<path>" << std::endl;
        return 1;
    }

try {
    utils::global_configuration = utils::configuration(config);
    if (ram_size) utils::sorter::RAM_SIZE = std::min(ram_size / 4, static_cast<size_t>(1e5));
    utils::sorter::sort(src, dst);
} catch (const std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return 1;
}
}