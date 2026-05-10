//
// Created by george on 10.05.2026.
//

#include "../include/sorter.hpp"


void utils::sorter::sort(tp& f1, tp& f2, tp& f3) {
    ram mem;
    mem.reserve(RAM_SIZE);
    int val;
    int cur_file = 0; // 0 -- 2, 1 -- 3
    size_t file_size = 0;
    while (f1.read(val)) {
        mem.push_back(val);
        if (mem.size() == RAM_SIZE) {
            std::sort(mem.begin(), mem.end());
            write_from_ram(mem, cur_file ? f3 : f2);
            cur_file = (cur_file + 1) & 1;
        }
        f1.shift_forward();
        file_size++;
    }
    write_from_ram(mem, cur_file ? f3 : f2);
    size_t bucket_size = RAM_SIZE;


    while (bucket_size < RAM_SIZE) {

    }

}

void utils::sorter::write_from_ram(ram& mem, tp& f) {
    for (auto &x : mem) {
        f.write(x);
        f.shift_forward();
    }
    mem.clear();
}
