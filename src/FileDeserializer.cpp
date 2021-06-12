#include "FileProcessor.h"
#include <iostream>
#include <sys/stat.h>
#include <FileDeserializer.h>
#include <string>
#include "FileCreator.h"
#include <fcntl.h>


namespace mini_tar {
    FileInfoViewer
    FileDeserializer::writeToDir(const std::string &parentPath, const FileInfo &fileInfo, std::istream &in) {
        std::string path = parentPath + "/" + fileInfo.name_;

        //std::cout << "inod sz - " << fileInfo.stat_.st_size << '\n';
        if (links.count({fileInfo.stat_.st_dev, fileInfo.stat_.st_ino})) {
            std::string &link = links[{fileInfo.stat_.st_dev, fileInfo.stat_.st_ino}];
            FileCreator::createHardlink(link, path);
            return FileInfoViewer(fileInfo, true);
        }
        if (S_ISREG(fileInfo.stat_.st_mode)) {
            FileCreator::createRegularFile(fileInfo, path, in);
        } else if (S_ISLNK(fileInfo.stat_.st_mode)) {
            FileCreator::createSymlink(fileInfo, path, in);
        } else if (S_ISDIR(fileInfo.stat_.st_mode)) {
            FileCreator::createDirectory(fileInfo, path);
        } else if (S_ISFIFO(fileInfo.stat_.st_mode)) {
            FileCreator::createFIFO(fileInfo, path);
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
}
