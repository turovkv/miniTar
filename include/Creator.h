#pragma once

#include <string>
#include <fstream>
#include "FileSerializer.h"

namespace mini_tar {
    class Creator {
    private:
        std::string src_;
        std::string dst_;
        std::ofstream ofs_;
        FileSerializer fs_;
        std::string archiveName;

    public:
        Creator(std::string_view src, std::string_view dst);

        void walk(std::string &path);
        void create();
    };
}
