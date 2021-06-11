#pragma once

#include <stdexcept>

namespace mini_tar {
    class TarException : public std::logic_error {
    public:
        explicit TarException(std::string_view message);
    };

    class MiniTar {

    };
}