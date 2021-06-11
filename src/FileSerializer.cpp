#include <cstring>
#include "FileProcessor.h"
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <FileSerializer.h>

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
        out << fileInfo.name_size_;
        out.write(fileInfo.name_, fileInfo.name_size_);
        out.write(reinterpret_cast<char *>(&fileInfo.stat_), sizeof(fileInfo.stat_));

        if (links.count({fileInfo.stat_.st_dev, fileInfo.stat_.st_ino})) { return; }
        if ((S_ISREG(fileInfo.stat_.st_mode) || S_ISLNK(fileInfo.stat_.st_mode))
            && fileInfo.stat_.st_size > 0) {
            std::ifstream in(path, std::ios::binary);
            out << in.rdbuf(); //??
        }
        links[{fileInfo.stat_.st_dev, fileInfo.stat_.st_ino}] = path;
    }

    void FileSerializer::write_up_flag(std::ostream &out) {
        FileInfo fileInfo;
        out << fileInfo.name_size_;
    }
}
