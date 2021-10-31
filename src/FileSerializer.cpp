#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <FileSerializer.h>
#include "FileProcessor.h"
#include "TarException.h"


namespace mini_tar {
    FileInfo FileSerializer::getFileInfo(const std::string &path) {
        FileInfo fileInfo;
        size_t begin = path.size();
        while (begin > 0 && path[begin - 1] != '/') {
            begin--;
        }
        for (size_t i = begin; i < path.size(); i++) {
            fileInfo.name_[i - begin] = path[i];
        }

        fileInfo.nameSize_ = path.size() - begin;

        if (lstat(path.c_str(), &fileInfo.stat_) < 0) {
            throw TarException("FileSerializer::getFileInfo: unable to get struct stat: " + path);
        }

        return fileInfo;
    }

    void FileSerializer::serialize(const std::string &path, std::ostream &out) {
        FileInfo fileInfo = getFileInfo(path);

        if (S_ISSOCK(fileInfo.stat_.st_mode)) {
            return;
        }
        out.write(reinterpret_cast<char *>(&fileInfo.nameSize_), sizeof(fileInfo.nameSize_));

        out.write(fileInfo.name_, fileInfo.nameSize_);
        out.write(reinterpret_cast<char *>(&fileInfo.stat_), sizeof(fileInfo.stat_));

        if (links.count({fileInfo.stat_.st_dev, fileInfo.stat_.st_ino})) { return; }
        if (S_ISREG(fileInfo.stat_.st_mode) && fileInfo.stat_.st_size > 0) {
            std::ifstream in(path, std::ios::binary);
            out << in.rdbuf();
        } else if (S_ISLNK(fileInfo.stat_.st_mode)) {
            const size_t bufferSize = 1 << 14;
            char buffer[bufferSize];
            ssize_t pathSize = readlink(path.c_str(), buffer, bufferSize);
            if (pathSize < 0) {
                throw TarException("FileSerializer::serialize: unable to read symlink: " + path);
            }
            out.write(buffer, pathSize);
        }
        links[{fileInfo.stat_.st_dev, fileInfo.stat_.st_ino}] = path;
    }

    void FileSerializer::writeUpFlag(std::ostream &out) {
        FileInfo fileInfo;
        out.write(reinterpret_cast<char *>(&fileInfo.nameSize_), sizeof(fileInfo.nameSize_));
    }
}
