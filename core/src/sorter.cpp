//
// Created by george on 10.05.2026.
//

#include "../include/sorter.hpp"


void utils::sorter::sort(tp& f1, tp& f2, tp& f3) {
    ram mem;
    mem.reserve(RAM_SIZE);
    size_t bucket_size = RAM_SIZE;
    size_t current_count = 1;
    while (bucket_size < f1.size_) {
        read_to_ram(f1,f2,f3,mem,current_count);
        write_buckets(f1,f2,f3);
        current_count <<= 1;
        bucket_size *= 2;
    }
}

void utils::sorter::write_from_ram(ram& mem, tp& f) {
    for (auto &x : mem) {
        f.write(x);
        f.shift_forward();
    }
    mem.clear();
}

void utils::sorter::write_buckets(tp& f1, tp& f2, tp& f3) {
    int val2, val3;
    while (1) {
        bool r2 = f2.read(val2);
        bool r3 = f3.read(val3);
        if (!r2 && !r3) {
            f1.rewind();
            f2.rewind();
            f3.rewind();
            return;
        }
        f1.write(!r3 || (r2 && val2 < val3) ? (f2.shift_forward(), val2) : (f3.shift_forward(), val3));
        f1.shift_forward();
    }
}

void utils::sorter::read_to_ram(tp& f1, tp& f2, tp& f3, ram& mem, size_t count) {

    size_t temp[] = {0,0};
    int cur_file = 0; // 0 -- 2, 1 -- 3
    int val;
    while (f1.read(val)) {
        mem.push_back(val);
        if (mem.size() == RAM_SIZE) {
            if (count == 1) // sorted after first run
                std::sort(mem.begin(), mem.end());
            write_from_ram(mem, cur_file ? f3 : f2);
            if (++temp[cur_file] == count) {
                temp[cur_file] = 0;
                cur_file = (cur_file + 1) & 1;
            }
        }
        f1.shift_forward();
    }
    write_from_ram(mem, cur_file ? f3 : f2);
    f1.rewind();
    f2.rewind();
    f3.rewind();
}

void utils::sorter::sort(tp& src, tp& dst) {
    int val;
    while (src.read(val)) {
        dst.write(val);
        src.shift_forward();
        dst.shift_forward();
    }
    tempfile temp1, temp2;
    tp f2(temp1), f3(temp2);
    sort(dst, f2,f3);
}
