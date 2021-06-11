#include <string>
#include "FileCreator.h"
#include "MiniTar.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>

namespace mini_tar {
    TarException FileCreator::getException(const std::string &what, const std::string &path) {
        return TarException("FileCreator: " + what + ": error = " + std::strerror(errno) + ", path = " + path);
    }

    void FileCreator::setMetadata(const FileInfo &fileInfo, const std::string &path) {
        if (lchown(path.c_str(), fileInfo.stat_.st_uid, fileInfo.stat_.st_gid) < 0) {
            throw getException("setMetadata: gid uid", path);
        }
        if (!S_ISLNK(fileInfo.stat_.st_mode) && chmod(path.c_str(), fileInfo.stat_.st_mode) < 0) {
            throw getException("setMetadata: mode", path);
        }
        struct timespec buf[2] = {fileInfo.stat_.st_atim, fileInfo.stat_.st_mtim};
        if (utimensat(AT_FDCWD, path.c_str(), buf, AT_SYMLINK_NOFOLLOW) < 0) {
            throw getException("setMetadata: time", path);
        }
    }

    void FileCreator::writeFileContent(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        std::ofstream ofs(path, std::ios::binary);
        for (int i = 0; i < fileInfo.stat_.st_size; i++) {
            ofs.put(in.get()); // TODO fuuuuuuuuuck
        }
        if (in.fail() || ofs.fail()) {
            throw getException("writeFileContent (reg file)", path);
        }
        ofs.close();
    }

    std::string
    FileCreator::getFileContent(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        std::string ans;
        for (int i = 0; i < fileInfo.stat_.st_size; i++) {
            ans.push_back(in.get()); // TODO fuuuuuuuuuck
        }
        if (in.fail()) {
            throw getException("getFileContent (symlink)", path);
        }
        return ans;
    }

    void
    FileCreator::createRegularFile(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        writeFileContent(fileInfo, path, in);
        setMetadata(fileInfo, path);
    }

    void FileCreator::createSymlink(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        std::string to = getFileContent(fileInfo, path, in);
        if (symlink(to.c_str(), path.c_str()) < 0) {
            throw getException("createSymlink", path);
        }
        setMetadata(fileInfo, path);
    }

    void FileCreator::createHardlink(const std::string &linkedFileName, const std::string &path) {
        if (link(linkedFileName.c_str(), path.c_str()) < 0) {
            throw getException("createHardlink", path);
        }
    }

    void FileCreator::createDirectory(const FileInfo &fileInfo, const std::string &path) {
        if (mkdir(path.c_str(), fileInfo.stat_.st_mode) < 0) {
            throw getException("createDirectory", path);
        }
        setMetadata(fileInfo, path);
    }

    void FileCreator::createFIFO(const FileInfo &fileInfo, const std::string &path) {
        if (mkfifo(path.c_str(), fileInfo.stat_.st_mode) < 0) {
            throw getException("createFIFO", path);
        }
        setMetadata(fileInfo, path);
    }
}
