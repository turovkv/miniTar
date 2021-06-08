#pragma once

#include <string>

namespace mini_tar {
    class Creator {
    private:
        std::string_view src_;
        std::string_view dst_;

    private:
        std::string get_src();

    public:
        Creator(std::string_view src, std::string_view dst);

        void walk(std::string &path, std::string pref = "");
        void create();

    };
}
