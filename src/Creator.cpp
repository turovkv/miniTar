#include <vector>
#include "Creator.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

namespace mini_tar {
    Creator::Creator(std::string_view src, std::string_view dst) {
        src_ = src;
        dst_ = dst;
    }

    void Creator::create() {
        std::string path(src_);
        walk(path);
    }

    void Creator::walk(std::string &path, std::string pref) {
        DIR *dir = opendir(path.c_str());
        struct dirent *dir_entry;
        while ((dir_entry = readdir(dir))) {
            std::string_view file_name(dir_entry->d_name);
            if (file_name == "." || file_name == "..") {
                continue;
            }

            int append_size = 1 + static_cast<int>(file_name.size());
            path.append("/").append(file_name);

            struct stat st{};
            if (lstat(path.c_str(), &st) < 0) {
                throw std::exception();
            }
            //if (S_ISREG(st.st_mode)) {
            std::cout << pref << file_name << '\n';
            //}
            if (S_ISDIR(st.st_mode)) {
                walk(path, pref + "---");
            }

            path.erase(path.size() - append_size, append_size);
        }
        closedir(dir);
    }
}
