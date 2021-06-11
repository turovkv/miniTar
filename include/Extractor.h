#pragma once

#include <string>
#include <fstream>
#include "FileSerializer.h"
#include "FileDeserializer.h"

namespace mini_tar {
    class Extractor {
    private:
        std::string src_;
        std::string dst_;
        std::ifstream ifs_;
        FileDeserializer fd_;

    private:
        static void moveToParentPath(std::string &path) ;

    public:
        Extractor(std::string_view src, std::string_view dst);

        void extract();
    };
}
