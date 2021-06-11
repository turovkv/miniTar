#include <vector>
#include "Extractor.h"
#include <dirent.h>
#include <iostream>

namespace mini_tar {
    Extractor::Extractor(std::string_view src, std::string_view dst) {
        src_ = src;
        dst_ = dst;
    }

    void Extractor::moveToParentPath(std::string &path) {
        while (path.back() != '/') {
            path.pop_back();
        }
        path.pop_back();
    }

    void Extractor::extract() {
        ifs_ = std::ifstream(dst_, std::ios::binary);

        std::string parentPath(src_);
        while (true) {
            FileInfoViewer fiw = fd_.deserialize(parentPath, ifs_);
            if (fiw.is_dir() && !fiw.is_hard_link()) {
                parentPath += "/" + fiw.name();
            }
            if (fiw.is_up_flag()) {
                if (parentPath == src_) {
                    break;
                }
                moveToParentPath(parentPath);
            }
        }
    }
}
