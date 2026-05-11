//
// Created by george on 10.05.2026.
//

#include "tempfile.hpp"
utils::tempfile::tempfile() {
   path_ =  std::tmpnam(nullptr);
   std::fstream creating(path_, std::ios::out);
}

utils::tempfile::~tempfile() {
    std::filesystem::remove(path_);
}

utils::tempfile::operator std::filesystem::path() {
    return path_;
}
