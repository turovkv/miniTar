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

    class File {
    public:
        uint32_t name_size_ = 0;
        char name_[256]{};
        struct stat st_{};

    public:
        File() = default;
        explicit File(std::istream &in);

        bool is_up_flag() const;
        bool is_dir() const;
    };

    class FileWriter : public File {
    private:
        static std::unordered_map<std::pair<ino_t, dev_t>, std::string, hash_pair> links;

    private:
        std::string path_;

    public:
        FileWriter() = default;
        explicit FileWriter(const std::string &path);
        void serialize(std::ostream &out);
    };

    class FileReader : public File {
    private:
        static std::unordered_map<std::pair<ino_t, dev_t>, std::string, hash_pair> links;

    private:
        std::string path_;

    public:
        FileReader(const std::string &parent_path, std::istream &in);
        void deserialize();
    };
}
