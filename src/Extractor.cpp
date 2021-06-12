#include <iostream>
#include "Extractor.h"

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
        inputStream_ = std::ifstream(dst_, std::ios::binary);

        std::string parentPath(src_);
        while (true) {
            FileInfoViewer fiw = fileDeserializer_.deserialize(parentPath, inputStream_);

            if (fiw.isDir() && !fiw.isHardLink()) {
                parentPath += "/" + fiw.getName();
            }
            if (fiw.isUpFlag()) {
                if (parentPath == src_) {
                    break;
                }
                moveToParentPath(parentPath);
            }
        }
        fileDeserializer_.setTimes();
    }
}
