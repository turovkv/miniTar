#include <cstring>
#include "FileProcessor.h"
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <FileSerializer.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include "MiniTar.h"


namespace mini_tar {
    FileInfo FileSerializer::get_file_info(const std::string &path) {
        FileInfo fileInfo;
        size_t begin = path.size();
        while (begin > 0 && path[begin - 1] != '/') {
            begin--;
        }
        for (size_t i = begin; i < path.size(); i++) {
            fileInfo.name_[i - begin] = path[i];
        }

        fileInfo.name_size_ = path.size() - begin;

        if (lstat(path.c_str(), &fileInfo.stat_) < 0) {
            throw std::exception(); //TODO
        }

        return fileInfo;
    }

    void FileSerializer::serialize(const std::string &path, std::ostream &out) {
        FileInfo fileInfo = get_file_info(path);
        //out << fileInfo.name_size_;

        if (S_ISSOCK(fileInfo.stat_.st_mode)) {
            return;
        }
        out.write(reinterpret_cast<char *>(&fileInfo.name_size_), sizeof(fileInfo.name_size_));

        out.write(fileInfo.name_, fileInfo.name_size_);
        out.write(reinterpret_cast<char *>(&fileInfo.stat_), sizeof(fileInfo.stat_));

        if (links.count({fileInfo.stat_.st_dev, fileInfo.stat_.st_ino})) { return; }
        if (S_ISREG(fileInfo.stat_.st_mode) && fileInfo.stat_.st_size > 0) {
            std::ifstream in(path, std::ios::binary);
            out << in.rdbuf(); //??
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

    void FileSerializer::write_up_flag(std::ostream &out) {
        FileInfo fileInfo;
        //out << fileInfo.name_size_;
        out.write(reinterpret_cast<char *>(&fileInfo.name_size_), sizeof(fileInfo.name_size_));
    }
}
