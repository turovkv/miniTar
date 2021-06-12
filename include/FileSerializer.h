#pragma once

#include <sys/stat.h>
#include <string>
#include <iosfwd>
#include <unordered_map>
#include "FileProcessor.h"

namespace mini_tar {
    class FileSerializer : public FileProcessor {
    private:
        static FileInfo getFileInfo(const std::string &path);

    public:
        FileSerializer() = default;
        static void writeUpFlag(std::ostream &out);
        void serialize(const std::string &path, std::ostream &out);
    };
}
