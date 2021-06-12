#include <TarException.h>
#include <Extractor.h>
#include <iostream>
#include <cstdio>
#include "CLI.h"
#include "Creator.h"

int main(int argc, char *argv[]) {
    try {
        mini_tar::CLI parser(argc, argv);
        try {
            if (parser.get_tar_mode() == mini_tar::CLI::CREATE) {
                mini_tar::Creator c(parser.get_dir_src(), parser.get_dir_dest());
                c.create();
            } else {
                mini_tar::Extractor e(parser.get_dir_src(), parser.get_dir_dest());
                e.extract();
            }
        } catch (mini_tar::TarException &e) {
            std::cout << e.what() << std::endl;
            std::remove(parser.get_dir_dest().data());
            return -2;
        }
    } catch (mini_tar::CLIException &e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
    return 0;
}
