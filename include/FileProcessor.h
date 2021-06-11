#pragma once

#include <sys/stat.h>
#include <string>
#include <iosfwd>
#include <unordered_map>

namespace mini_tar {
    struct hash_pair {
        template<class T1, class T2>
        size_t operator()(const std::pair<T1, T2> &p) const {
            auto hash1 = std::hash<T1>{}(p.first);
            auto hash2 = std::hash<T2>{}(p.second);
            return hash1 ^ hash2;
        }
    };

    class FileInfo {
    public:
        uint32_t name_size_{};
        char name_[256]{};
        struct stat stat_{};

    public:
        bool is_up_flag() const;
        bool is_dir() const;
    };

    class FileInfoViewer {
    private:
        FileInfo file_info_;

    public:
        FileInfoViewer() = delete;
        explicit FileInfoViewer(FileInfo fileInfo);
        bool is_up_flag() const;
        bool is_dir() const;
    };

    class FileProcessor {
    protected:
        std::unordered_map<std::pair<ino_t, dev_t>, std::string, hash_pair> links{};
    };
}
