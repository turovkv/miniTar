#include <cstring>
#include "File.h"
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fstream>

namespace mini_tar {
    bool File::is_up_flag() const {
        return name_size_ == 0;
    }

    bool File::is_dir() const {
        return S_ISDIR(st_.st_mode);
    }

    std::unordered_map<std::pair<ino_t, dev_t>, std::string, hash_pair> FileWriter::links;

    FileWriter::FileWriter(const std::string &path) {
        size_t begin = path.size();
        while (begin > 0 && path[begin - 1] != '/') {
            begin--;
        }
        for (size_t i = begin; i < path.size(); i++) {
            name_[i - begin] = path[i];
        }

        name_size_ = path.size() - begin;

        if (lstat(path.c_str(), &st_) < 0) {
            throw std::exception(); //TODO
        }

        path_ = path;
    }

    void FileWriter::serialize(std::ostream &out) {
        out << name_size_;
        if (is_up_flag()) {
            return;
        }
        out.write(name_, name_size_);
        out.write(reinterpret_cast<char *>(&st_), sizeof(st_));

        if (!links.count({st_.st_dev, st_.st_ino})) {
            if (S_ISREG(st_.st_mode) && st_.st_size > 0) {
                std::ifstream in(path_, std::ios::binary);
                out << in.rdbuf();
            }
            links[{st_.st_dev, st_.st_ino}] = path_;
        }
    }

    FileReader::FileReader(const std::string &parent_path, std::istream &in) {
        in >> name_size_;
        if (name_size_ == 0) {
            return;
        }
        in.read(name_, name_size_);
        in.read(reinterpret_cast<char *>(&st_), sizeof(st_));

        path_ = std::string()
                .append(parent_path)
                .append("/")
                .append(name_);
    }

    void FileReader::deserialize() {

    }
}
