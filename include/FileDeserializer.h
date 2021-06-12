#pragma once

#include <sys/stat.h>
#include <string>
#include <iosfwd>
#include <unordered_map>
#include <vector>
#include <functional>
#include "FileProcessor.h"
#include "TarException.h"

namespace mini_tar {
    class FileDeserializer : public FileProcessor {
    private:
        std::vector<std::function<void()>> timeChangeQueue;
        FileInfoViewer writeToDir(const std::string &parentPath, const FileInfo &fileInfo, std::istream &in);

    private:
        TarException getException(const std::string &what, const std::string &path);

        void setMetadata(const FileInfo &fileInfo, const std::string &path);
        void writeFileContent(const FileInfo &fileInfo, const std::string &path, std::istream &in);
        std::string getSymLinkPath(const FileInfo &fileInfo, const std::string &path, std::istream &in);

        void createHardlink(const std::string &linkedFileName, const std::string &path);
        void createRegularFile(const FileInfo &fileInfo, const std::string &path, std::istream &in);
        void createSymlink(const FileInfo &fileInfo, const std::string &path, std::istream &in);
        void createDirectory(const FileInfo &fileInfo, const std::string &path);
        void createFIFO(const FileInfo &fileInfo, const std::string &path);

    public:
        FileDeserializer() = default;
        void setTimes();
        FileInfoViewer deserialize(const std::string &parentPath, std::istream &in);
    };
}
