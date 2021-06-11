#pragma once

#include <sys/stat.h>
#include <string>
#include <iosfwd>
#include <unordered_map>
#include "FileProcessor.h"
#include "MiniTar.h"

namespace mini_tar {
    class FileDeserializer : public FileProcessor {
    private:
        FileInfoViewer writeToDir(const std::string &parentPath, const FileInfo &fileInfo, std::istream &in);

    public:
        FileDeserializer() = default;
        FileInfoViewer deserialize(const std::string &parentPath, std::istream &in);
    };
}
