#include "FileProcessor.h"
#include <iostream>
#include <sys/stat.h>
#include <FileDeserializer.h>
#include <string>
#include <fcntl.h>
#include <string>
#include "MiniTar.h"
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <cerrno>
#include <cstring>


namespace mini_tar {
    FileInfoViewer
    FileDeserializer::writeToDir(const std::string &parentPath, const FileInfo &fileInfo, std::istream &in) {
        std::string path = parentPath + "/" + fileInfo.name_;

        //std::cout << "inod sz - " << fileInfo.stat_.st_size << '\n';
        if (links.count({fileInfo.stat_.st_dev, fileInfo.stat_.st_ino})) {
            std::string &link = links[{fileInfo.stat_.st_dev, fileInfo.stat_.st_ino}];
            createHardlink(link, path);
            return FileInfoViewer(fileInfo, true);
        }
        if (S_ISREG(fileInfo.stat_.st_mode)) {
            createRegularFile(fileInfo, path, in);
        } else if (S_ISLNK(fileInfo.stat_.st_mode)) {
            createSymlink(fileInfo, path, in);
        } else if (S_ISDIR(fileInfo.stat_.st_mode)) {
            createDirectory(fileInfo, path);
        } else if (S_ISFIFO(fileInfo.stat_.st_mode)) {
            createFIFO(fileInfo, path);
        }
        links[{fileInfo.stat_.st_dev, fileInfo.stat_.st_ino}] = path;
        return FileInfoViewer(fileInfo, false);
    }

    FileInfoViewer FileDeserializer::deserialize(const std::string &parentPath, std::istream &in) {
        FileInfo fileInfo;
        //in >> fileInfo.name_size_;
        in.read(reinterpret_cast<char *>(&fileInfo.name_size_), sizeof(fileInfo.name_size_));
        //std::cout << "sz - " << fileInfo.name_size_ << '\n';

        if (fileInfo.is_up_flag()) {
            //std::cout << "UP\n";
            return FileInfoViewer(fileInfo, false);
        }
        in.read(fileInfo.name_, fileInfo.name_size_);
        //std::cout << "name - " << fileInfo.name_ << '\n';
        in.read(reinterpret_cast<char *>(&fileInfo.stat_), sizeof(fileInfo.stat_));
        ////std::cout << "read name > " << fileInfo.name_ << '\n';

        return writeToDir(parentPath, fileInfo, in);
    }

    TarException FileDeserializer::getException(const std::string &what, const std::string &path) {
        return TarException("FileDeserializer: " + what + ": error = " + std::strerror(errno) + ", path = " + path);
    }

    void FileDeserializer::setTimes() {
        while (!timeChangeQueue.empty()) {
            timeChangeQueue.back()();
            timeChangeQueue.pop_back();
        }
    }

    void FileDeserializer::setMetadata(const FileInfo &fileInfo, const std::string &path) {
        if (lchown(path.c_str(), fileInfo.stat_.st_uid, fileInfo.stat_.st_gid) < 0) {
            throw getException("setMetadata: gid uid", path);
        }
        if (!S_ISLNK(fileInfo.stat_.st_mode) && chmod(path.c_str(), fileInfo.stat_.st_mode) < 0) {
            throw getException("setMetadata: mode", path);
        }
        struct timespec buf[2] = {fileInfo.stat_.st_atim, fileInfo.stat_.st_mtim};

        timeChangeQueue.emplace_back([buf, path, this]() {
            if (utimensat(AT_FDCWD, path.c_str(), buf, AT_SYMLINK_NOFOLLOW) < 0) {
                throw getException("setMetadata: time", path);
            }
        });
    }

    void FileDeserializer::writeFileContent(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
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
    FileDeserializer::getSymLinkPath(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        std::string ans;
        for (int i = 0; i < fileInfo.stat_.st_size; i++) {
            ans.push_back(in.get()); // TODO fuuuuuuuuuck
        }
        if (in.fail()) {
            throw getException("getSymLinkPath (symlink)", path);
        }
        //std::cout << "smlk - " << ans << '\n';

        return ans;
    }

    void
    FileDeserializer::createRegularFile(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        writeFileContent(fileInfo, path, in);
        setMetadata(fileInfo, path);
    }

    void FileDeserializer::createSymlink(const FileInfo &fileInfo, const std::string &path, std::istream &in) {
        std::string to = getSymLinkPath(fileInfo, path, in);
        if (symlink(to.c_str(), path.c_str()) < 0) {
            throw getException("createSymlink", path);
        }
        setMetadata(fileInfo, path);
    }

    void FileDeserializer::createHardlink(const std::string &linkedFileName, const std::string &path) {
        if (link(linkedFileName.c_str(), path.c_str()) < 0) {
            throw getException("createHardlink", path);
        }
    }

    void FileDeserializer::createDirectory(const FileInfo &fileInfo, const std::string &path) {
        if (mkdir(path.c_str(), fileInfo.stat_.st_mode) < 0) {
            throw getException("createDirectory", path);
        }
        setMetadata(fileInfo, path);
    }

    void FileDeserializer::createFIFO(const FileInfo &fileInfo, const std::string &path) {
        if (mkfifo(path.c_str(), fileInfo.stat_.st_mode) < 0) {
            throw getException("createFIFO", path);
        }
        setMetadata(fileInfo, path);
    }
}
