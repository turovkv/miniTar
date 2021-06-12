#include <sys/stat.h>
#include <fcntl.h>
#include "FileProcessor.h"

namespace mini_tar {
    FileInfoViewer::FileInfoViewer(FileInfo fileInfo, bool isHardLink) {
        fileInfo_ = fileInfo;
        isHardLink_ = isHardLink;
    }

    bool FileInfo::isUpFlag() const {
        return nameSize_ == 0;
    }

    bool FileInfo::isDir() const {
        return S_ISDIR(stat_.st_mode);
    }

    bool FileInfoViewer::isUpFlag() const {
        return fileInfo_.isUpFlag();
    }

    bool FileInfoViewer::isDir() const {
        return fileInfo_.isDir();
    }

    bool FileInfoViewer::isHardLink() const {
        return isHardLink_;
    }

    std::string FileInfoViewer::getName() const {
        return std::string(fileInfo_.name_);
    }
}
