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
    static void process_with_counts(tp& f1, tp& f2, tp& f3, ram& mem, size_t count, size_t& total2, size_t& total3);
    static void merge_buckets(tp& f1, tp& f2, tp& f3, size_t bucket_size, size_t total2, size_t total3);

public:
    static size_t RAM_SIZE;
    static void sort(tp& src, tp& dst);
    static void sort(const std::string &src, const std::string &dst);
};

}
