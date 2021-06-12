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
//            //std::cout << "deserialize - " << fiw.name() << ' ';
//            //std::cout << "is dir - " << fiw.is_dir() << ' ';
//            //std::cout << "is up - " << fiw.is_up_flag() << ' ';
//            //std::cout << "is hl - " << fiw.is_hard_link() << '\n';
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
