//
// Created by george on 10.05.2026.
//
#pragma once
#include "tempfile.hpp"
#include "../include/tape_processer.hpp"
#include <vector>

namespace utils {
    class tape_processer;

class sorter {
    using tp = tape_processer;
    using ram = std::vector<int>;

    // work, temp1, temp2
    static void sort(tp& f1, tp& f2, tp& f3);
    static void write_from_ram(ram &mem, tp &f);
    static void write_buckets(tp &f1, tp &f2, tp &f3);
    static void read_to_ram(tp &f1, tp &f2, tp &f3, ram &mem, size_t count);

public:
    static size_t RAM_SIZE;
    static void sort(tp& src, tp& dst);
};

}
