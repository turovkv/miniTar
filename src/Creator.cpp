#include <vector>
#include <dirent.h>
#include <iostream>
#include "Creator.h"

namespace mini_tar {
    Creator::Creator(std::string_view src, std::string_view dst) {
        src_ = src;
        dst_ = dst;
        size_t begin = dst.size();
        while (begin > 0 && dst[begin - 1] != '/') {
            begin--;
        }
        for (size_t i = begin; i < dst.size(); i++) {
            archiveName.push_back(dst[i]);
        }
    }

    void Creator::create() {
        outputStream_ = std::ofstream(dst_, std::ios::binary);
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
            if (file_name == "." || file_name == ".." || file_name == archiveName) {
                continue;
            }

            size_t append_size = 1 + file_name.size();
            path.append("/").append(file_name);

            fileSerializer_.serialize(path, outputStream_);
            walk(path);

            path.erase(path.size() - append_size, append_size);
        }
        mini_tar::FileSerializer::writeUpFlag(outputStream_);
        closedir(dir);
    }
}
