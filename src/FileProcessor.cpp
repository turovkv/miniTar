#include "FileProcessor.h"
#include <sys/stat.h>
#include <fcntl.h>

namespace mini_tar {
    FileInfoViewer::FileInfoViewer(FileInfo fileInfo) {
        file_info_ = fileInfo;
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
}
