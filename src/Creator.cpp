#include <vector>
#include "Creator.h"
#include <dirent.h>
#include <iostream>

namespace mini_tar {
    Creator::Creator(std::string_view src, std::string_view dst) {
        src_ = src;
        dst_ = dst;
    }

    void Creator::create() {
        ofs_ = std::ofstream(dst_, std::ios::binary);
        walk(src_);
    }

    void Creator::walk(std::string &path) {
        DIR *dir = opendir(path.c_str());
        if (dir == nullptr) {
            return; // TODO errno ENOTDIR
        }
        struct dirent *dir_entry;
        while ((dir_entry = readdir(dir))) {
            std::string_view file_name(dir_entry->d_name);
            if (file_name == "." || file_name == "..") {
                continue;
            }

            size_t append_size = 1 + file_name.size();
            path.append("/").append(file_name);

            fs_.serialize(path, ofs_);
            walk(path);

            path.erase(path.size() - append_size, append_size);
        }
        mini_tar::FileSerializer::write_up_flag(ofs_);
        closedir(dir);
    }
}
