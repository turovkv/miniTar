#include <iostream>
#include <MiniTar.h>
#include "CLI.h"
#include "Creator.h"

int main(int argc, char* argv[]) {
    try {
        mini_tar::CLI parser(argc, argv);
        std::cout << parser.get_tar_mode() << '\n';
        std::cout << parser.get_dir_src() << '\n';
        std::cout << parser.get_dir_dest() << '\n';
        if (parser.get_tar_mode() == mini_tar::CLI::CREATE) {
            mini_tar::Creator c(
                    parser.get_dir_src(),
                    parser.get_dir_dest()
                    );
            c.create();
        } else {
            std::cout << "lol\n";
        }
    } catch (mini_tar::CLIException &e) {
        std::cout << e.what() << std::endl;
        return -2;
    } catch (mini_tar::TarException &e) {
        std::cout << e.what() << std::endl;
        return -1;
    } catch (...) {
        throw ;
    }
    return 0;
}
