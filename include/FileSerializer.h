#pragma once

#include <sys/stat.h>
#include <string>
#include <iosfwd>
#include <unordered_map>
#include "FileProcessor.h"

namespace mini_tar {
    class FileSerializer : public FileProcessor {
    private:
        static FileInfo get_file_info(const std::string &path);

    public:
        FileSerializer() = default;
        static void write_up_flag(std::ostream &out);
        void serialize(const std::string &path, std::ostream &out);
    };
}
