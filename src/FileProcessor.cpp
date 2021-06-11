#include "FileProcessor.h"
#include <sys/stat.h>
#include <fcntl.h>

namespace mini_tar {
    FileInfoViewer::FileInfoViewer(FileInfo fileInfo, bool is_hard_link) {
        file_info_ = fileInfo;
        is_hard_link_ = is_hard_link;
    }

    bool FileInfo::is_up_flag() const {
        return name_size_ == 0;
    }

    bool FileInfo::is_dir() const {
        return S_ISDIR(stat_.st_mode);
    }

    bool FileInfoViewer::is_up_flag() const {
        return file_info_.is_up_flag();
    }

    bool FileInfoViewer::is_dir() const {
        return file_info_.is_dir();
    }

    bool FileInfoViewer::is_hard_link() const {
        return is_hard_link_;
    }

    std::string FileInfoViewer::name() const {
        return std::string(file_info_.name_);
    }
}
