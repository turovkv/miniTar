#pragma once

#include "FileProcessor.h"
#include "MiniTar.h"

namespace mini_tar {
    class FileCreator {
    public:
        static TarException getException(const std::string &what, const std::string &path);

        static void setMetadata(const FileInfo &fileInfo, const std::string &path);
        static void writeFileContent(const FileInfo &fileInfo, const std::string &path, std::istream &in);
        static std::string getSymLinkPath(const FileInfo &fileInfo, const std::string &path, std::istream &in);

        static void createHardlink(const std::string &linkedFileName, const std::string &path);
        static void createRegularFile(const FileInfo &fileInfo, const std::string &path, std::istream &in);
        static void createSymlink(const FileInfo &fileInfo, const std::string &path, std::istream &in);
        static void createDirectory(const FileInfo &fileInfo, const std::string &path);
        static void createFIFO(const FileInfo &fileInfo, const std::string &path);
    };
}
