//
// Created by george on 10.05.2026.
//

#include "sorter.hpp"

void utils::sorter::sort(tp& f1, tp& f2, tp& f3) {
    ram mem;
    mem.reserve(RAM_SIZE);

    size_t total_f2 = 0, total_f3 = 0;
    process_with_counts(f1, f2, f3, mem, 1, total_f2, total_f3);

    size_t bucket_size = RAM_SIZE;

    while ((total_f2 > 0 && total_f3 > 0) || bucket_size == RAM_SIZE /* at least one run*/) {
        merge_buckets(f1, f2, f3, bucket_size, total_f2, total_f3);
        bucket_size *= 2;
        process_with_counts(f1, f2, f3, mem, bucket_size / RAM_SIZE, total_f2, total_f3);
    }
    f1.rewind();
}

void utils::sorter::process_with_counts(tp& f1, tp& f2, tp& f3, ram& mem, size_t count, size_t& total2, size_t& total3) {
    f1.rewind(); f2.rewind(); f3.rewind();
    total2 = 0; total3 = 0;
    int cur_file = 0;
    int val;
    size_t buckets_in_cur = 0;

    auto flush = [&]() -> void {
        if (mem.empty()) return;

        if (count == 1) // has relative order after the first run
            std::sort(mem.begin(), mem.end());

        for (int x : mem) (cur_file ? f3 : f2).write(x), (cur_file ? f3 : f2).shift_forward();
        (cur_file ? total3 : total2) += mem.size();
        mem.clear();
        buckets_in_cur++;

        if (buckets_in_cur == count) {
            cur_file = 1 - cur_file;
            buckets_in_cur = 0;
        }
    };

    while (f1.read(val)) {
        mem.push_back(val);
        if (mem.size() == RAM_SIZE) flush();
        f1.shift_forward();
    }
    flush();
}

void utils::sorter::merge_buckets(tp& f1, tp& f2, tp& f3, size_t bucket_size, size_t limit2, size_t limit3) {
    f1.rewind(), f2.rewind(), f3.rewind();
    auto dec = [](size_t &x) -> void {x = std::min(x, x - 1);};

    while (limit2 || limit3) {
        // не хочется брать мусор, поэтому заметим, что если на ленту был записан не полный бакет, то обязательно последним
        size_t left2 = std::min(bucket_size, limit2);
        size_t left3 = std::min(bucket_size, limit3);

        int v2, v3;
        while (true) {
            bool has2 = left2 && f2.read(v2);
            bool has3 = left3 && f3.read(v3);
            if (!has2 && !has3) break;
            f1.write(!has3 || (has2 && v2 < v3) ?
                (dec(left2), dec(limit2), f2.shift_forward(), v2)
                :
                (dec(left3), dec(limit3), f3.shift_forward(), v3));
            f1.shift_forward();
        }
    }
}

void utils::sorter::sort(tp& src, tp& dst) {
    tempfile t1, t2;
    tp f2(t1), f3(t2);
    int val;
    while (src.read(val)) dst.write(val), dst.shift_forward(), src.shift_forward();
    dst.rewind();
    sort(dst, f2,f3);
    src.rewind();
    dst.rewind();
}

void utils::sorter::sort(const std::string& src, const std::string& dst) {
    tp src_(src), dst_(dst);
    sort(src_, dst_);
}

size_t utils::sorter::RAM_SIZE = 20;
