#include <TarException.h>
#include <Extractor.h>
#include <iostream>
#include <cstdio>
#include "CLI.h"
#include "Creator.h"
#include "HuffmanArchiver.h"


int main(int argc, char *argv[]) {
    try {
        mini_tar::CLI parser(argc, argv);
        try {
            std::string src_folder = std::string(parser.get_dir_src());
            std::string dest_file_compressed = std::string(parser.get_dir_dest());
            std::string dest_file_uncompressed = dest_file_compressed + "_unc";
            if (parser.get_tar_mode() == mini_tar::CLI::CREATE) {
                mini_tar::Creator(src_folder, dest_file_uncompressed).create();
                HuffmanArchiver::HuffmanArchiver().compress(dest_file_uncompressed, dest_file_compressed);
                std::remove(dest_file_uncompressed.c_str());
            } else {
                HuffmanArchiver::HuffmanArchiver().extract(dest_file_compressed, dest_file_uncompressed);
                mini_tar::Extractor(src_folder, dest_file_uncompressed).extract();
                std::remove(dest_file_uncompressed.c_str());
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
