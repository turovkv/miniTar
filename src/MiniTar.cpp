#include "MiniTar.h"
namespace mini_tar {
    TarException::TarException(std::string_view message)
            : std::logic_error(message.data()) {}
}
