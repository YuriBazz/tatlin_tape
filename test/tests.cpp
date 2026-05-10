//
// Created by george on 09.05.2026.
//
#include "../libs/simple_test.h"
#include "../core/include/configuration.hpp"
#include "../core/include/tape_processer.hpp"
#include "../core/include/tempfile.hpp"


using path = std::filesystem::path;
using json = nlohmann::json;

const path configs_path = CONFIGS_DIR;
const path tapes_path = TAPES_DIR;

namespace {

void write_tape(const path &tape_path, const std::vector<int> &values) {
    std::fstream out(tape_path, std::ios::out | std::ios::binary | std::ios::trunc);
    for (int value : values) {
        out.write(reinterpret_cast<const char *>(&value), sizeof(value));
    }
}

std::vector<int> read_all(utils::tape_processer &tp) {
    std::vector<int> values;
    int value;
    while (tp.read(value)) {
        values.push_back(value);
        tp.shift_forward();
    }
    return values;
}

void assert_tape_eq(const std::vector<int> &actual, const std::vector<int> &expected) {
    ASSERT_EQ(actual.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(actual[i], expected[i]);
    }
}

}

TEST(Configuration, non_existing_config) {
    ASSERT_NO_THROW(try {
        utils::global_configuration = configs_path / "zzzzzeros.json";
    }catch (const utils::configuration_error &ex) {
        //....
    });
    utils::global_configuration = utils::configuration();
}

TEST(Configuration, incomplete_config) {
    std::ostringstream os;
    try {
        utils::global_configuration = configs_path / "incomplete.json";
    }catch (const utils::configuration_error &ex) {
        os << ex.what();
    }
    ASSERT_EQ(os.str(), "Incomplete config.json");
    utils::global_configuration = utils::configuration();
}

TEST(Configuration, too_big_config) {
    std::ostringstream os;
    try {
        utils::global_configuration = configs_path / "big.json";
    }catch (const utils::configuration_error &ex) {
        os << ex.what();
    }
    ASSERT_EQ(os.str(), "Too much fields in config.json");
    utils::global_configuration = utils::configuration();
}

TEST(Configuration, wrong_config) {
    std::exception_ptr e_ptr = nullptr;
    ASSERT_NO_THROW(try {
        utils::global_configuration = configs_path / "wrong.json";
    }catch (...) {
        e_ptr = std::current_exception();
    });
    ASSERT_TRUE(e_ptr != nullptr);
    utils::global_configuration = utils::configuration();
}

TEST(Configuration, no_config) {
    ASSERT_EQ(utils::global_configuration.reading_delay, std::chrono::microseconds(0));
    ASSERT_EQ(utils::global_configuration.writing_delay, std::chrono::microseconds(0));
    ASSERT_EQ(utils::global_configuration.shift_delay, std::chrono::microseconds(0));
    ASSERT_EQ(utils::global_configuration.rewind_delay, std::chrono::microseconds(0));
    utils::global_configuration = utils::configuration();
}



TEST(Tape_processer, empty_empty_config) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {});
    utils::tape_processer tp(std::move(tmp_path));
    ASSERT_FALSE(tp.eof());
    assert_tape_eq(read_all(tp), {});
    ASSERT_TRUE(tp.eof());
}

TEST(Tape_processer, reads_binary_ints_until_eof) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {5, -2, 0, 1024});

    utils::tape_processer tp(tmp);

    assert_tape_eq(read_all(tp), {5, -2, 0, 1024});
    ASSERT_TRUE(tp.eof());
}

TEST(Tape_processer, rewind_allows_rereading_from_beginning) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {7, 8, 9});

    utils::tape_processer tp(std::move(tmp_path));

    int value;
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 7);

    tp.rewind();
    ASSERT_FALSE(tp.eof());
    assert_tape_eq(read_all(tp), {7, 8, 9});
}

TEST(Tape_processer, shift_forward_moves_to_next_int) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {10, 20, 30, 40});

    utils::tape_processer tp(std::move(tmp_path));

    int value;
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 10);

    tp.shift_forward();

    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 20);
    tp.shift_forward();
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 30);
    tp.shift_forward();
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 40);
}

TEST(Tape_processer, shift_backward_returns_one_int_back) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {10, 20, 30, 40});

    utils::tape_processer tp(std::move(tmp_path));

    tp.shift_forward();
    tp.shift_forward();

    int value;
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 30);

    tp.shift_backward();

    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 20);
    tp.shift_backward();
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 10);
}

TEST(Tape_processer, shift_forward_and_backward_can_walk_the_tape) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {4, 8, 15, 16, 23, 42});

    utils::tape_processer tp(std::move(tmp_path));

    int value;
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 4);

    tp.shift_forward();
    tp.shift_forward();
    tp.shift_forward();
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 16);

    tp.shift_backward();
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 15);

    tp.shift_forward();
    tp.shift_forward();
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 23);
}

TEST(Tape_processer, rewind_resets_position_after_shifts) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {100, 200, 300});

    utils::tape_processer tp(std::move(tmp_path));

    tp.shift_forward();
    tp.shift_forward();

    int value;
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 300);

    tp.rewind();

    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 100);
}

TEST(Tape_processer, write_overwrites_current_position_after_read) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {1, 2, 3});

    utils::tape_processer tp(std::move(tmp_path));

    int value;
    ASSERT_TRUE(tp.read(value));
    ASSERT_EQ(value, 1);
    tp.write(42);

    tp.rewind();
    assert_tape_eq(read_all(tp), {42, 2, 3});
}

TEST(Tape_processer, write_after_shift_overwrites_selected_cell) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {1, 2, 3});

    utils::tape_processer tp(std::move(tmp_path));

    tp.shift_forward();
    tp.write(42);

    tp.rewind();
    assert_tape_eq(read_all(tp), {1, 42, 3});
}

TEST(Tape_processer, move_constructor_preserves_position_and_data) {
    utils::tempfile tmp;
    path tmp_path = tmp;
    write_tape(tmp_path, {3, 6, 9});

    utils::tape_processer src(std::move(tmp_path));

    int value;
    ASSERT_TRUE(src.read(value));
    ASSERT_EQ(value, 3);
    src.shift_forward();

    utils::tape_processer moved(std::move(src));

    ASSERT_TRUE(moved.read(value));
    ASSERT_EQ(value, 6);
    moved.shift_forward();
    ASSERT_TRUE(moved.read(value));
    ASSERT_EQ(value, 9);
}

TEST(Tape_processer, move_assignment_preserves_position_and_data) {
    utils::tempfile src_tmp;
    utils::tempfile dst_tmp;
    path src_path = src_tmp;
    path dst_path = dst_tmp;
    write_tape(src_path, {11, 22, 33});
    write_tape(dst_path, {44});

    utils::tape_processer src(std::move(src_path));
    utils::tape_processer dst(std::move(dst_path));

    int value;
    ASSERT_TRUE(src.read(value));
    ASSERT_EQ(value, 11);
    src.shift_forward();

    dst = std::move(src);

    ASSERT_TRUE(dst.read(value));
    ASSERT_EQ(value, 22);
    dst.shift_forward();
    ASSERT_TRUE(dst.read(value));
    ASSERT_EQ(value, 33);
}

TEST(Tape_processer, numbers_empty_config_write_from_src_to_dst) {
    utils::tape_processer src(tapes_path / "numbers.txt");
    auto tmp = utils::tempfile();
    utils::tape_processer dst(tmp);
    int val;
    std::ostringstream os_src;
    while (src.read(val)) {
        os_src << val;
        src.shift_forward();
    }

    src.rewind();

    while (src.read(val)) {
        dst.write(val);
        src.shift_forward();
        dst.shift_forward();
    }
    ASSERT_TRUE(src.eof());
    dst.rewind();
    ASSERT_FALSE(dst.eof());

    std::ostringstream os_dst;
    while (dst.read(val)) {
        os_dst << val;
        dst.shift_forward();
    }
    ASSERT_TRUE(dst.eof());
    ASSERT_EQ(os_dst.str(), os_src.str());
}

// В текущей реализации, eof не будет нормальным чекером состояния файла.
TEST(Tape_processer, eof_could_not_be_condiiton) {
    utils::tape_processer src(tapes_path / "numbers.txt");
    auto tmp = utils::tempfile();
    utils::tape_processer dst(tmp);
    int val;
    std::ostringstream os_src;
    while (!src.eof()) {
        src.read(val);
        os_src << val;
        src.shift_forward();
    }

    src.rewind();

    while (src.read(val)) {
        dst.write(val);
        src.shift_forward();
        dst.shift_forward();
    }
    ASSERT_TRUE(src.eof());
    dst.rewind();
    ASSERT_FALSE(dst.eof());

    std::ostringstream os_dst;
    while (dst.read(val)) {
        os_dst << val;
        dst.shift_forward();
    }
    ASSERT_TRUE(dst.eof());
    ASSERT_NE(os_dst.str(), os_src.str());
}


TEST(Tape_processer, write_to_not_exisiting_file) {
    if (std::filesystem::exists(tapes_path/"out")) {
        std::filesystem::remove(tapes_path / "out");
    }
    utils::tape_processer src(tapes_path/ "numbers.txt");
    utils::tape_processer dst(tapes_path / "out");
    int val;
    std::ostringstream os_src;
    while (src.read(val)) {
        os_src << val;
        src.shift_forward();
    }

    src.rewind();

    while (src.read(val)) {
        dst.write(val);
        src.shift_forward();
        dst.shift_forward();
    }
    ASSERT_TRUE(src.eof());
    dst.rewind();
    ASSERT_FALSE(dst.eof());

    std::ostringstream os_dst;
    while (dst.read(val)) {
        os_dst << val;
        dst.shift_forward();
    }
    ASSERT_TRUE(dst.eof());
    ASSERT_EQ(os_dst.str(), os_src.str());
    std::filesystem::remove(tapes_path / "out");
}

TESTING_MAIN()
