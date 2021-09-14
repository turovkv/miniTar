//
// Created by kirill on 12/04/2020.
//

#include <iostream>
#include <fstream>
#include "HuffmanArchiver.h"

namespace HuffmanArchiver {
    HuffmanArchiverException::HuffmanArchiverException(std::string_view message) :
            std::runtime_error(message.data()) {}

    int64_t HuffmanArchiver::compress(std::string_view src, std::string_view dst) {
        std::ifstream is(src.data(), std::ios::binary);
        std::ofstream os(dst.data(), std::ios::binary);
        return compress(is, os);
    }

    int64_t HuffmanArchiver::compress(std::istream &is, std::ostream &os) {
        BitStream::iBitStream b_in(is);
        BitStream::oBitStream b_out(os);

        std::map<uint8_t, int64_t> frequency;
        int64_t count_symbols = 0; //number of symbols of uncompressed file

        while (b_in) {
            uint8_t byte = 0;
            b_in.read(&byte, 1);
            frequency[byte]++;
            count_symbols++;
        }
        if (frequency.empty()) { // топ костыль
            frequency[0] = 0;
            frequency[1] = 0;
        }
        if (frequency.size() == 1) {
            uint8_t byte = frequency.begin()->first;
            frequency[byte + 1] = 0;
        }

        if (is.bad()) {
            throw HuffmanArchiverException("HuffmanArchiver::compress(...): input error");
        }

        HuffmanTree tree(frequency);
        std::map<uint8_t, vector<bool>> codes = tree.get_all_codes();
        auto [leaves, traverse] = tree.get_tree_info();

        int32_t count_leaves = leaves.size();

        b_out.write(reinterpret_cast<uint8_t *>(&count_symbols), sizeof(count_symbols));
        b_out.write(reinterpret_cast<uint8_t *>(&count_leaves), sizeof(count_leaves));
        for (uint8_t leaf : leaves) {
            b_out.write(&leaf, 1);
        }
        for (bool event : traverse) {
            b_out << event;
        }

        b_in.reopen();
        while (b_in) {
            uint8_t byte = 0;
            b_in.read(&byte, 1);
            for (auto bit : codes[byte]) {
                b_out << bit;
            }
        }

        b_out.close();
        b_in.close();

        int64_t header = 0;
        header += sizeof(count_symbols);
        header += sizeof(count_leaves);
        header += count_leaves;
        header += (count_leaves - 1) / 2;
        return header;
    }

    int64_t HuffmanArchiver::extract(std::string_view src, std::string_view dst) {
        std::ifstream is(src.data(), std::ios::binary);
        std::ofstream os(dst.data(), std::ios::binary);
        return extract(is, os);
    }

    int64_t HuffmanArchiver::extract(std::istream &is, std::ostream &os) {
        BitStream::iBitStream b_in(is);
        BitStream::oBitStream b_out(os);

        int64_t count_symbols = 0; //number of symbols of uncompressed file
        int32_t count_leaves = 0;
        b_in.read(reinterpret_cast<uint8_t *>(&count_symbols), sizeof(count_symbols));
        b_in.read(reinterpret_cast<uint8_t *>(&count_leaves), sizeof(count_leaves));

        vector<uint8_t> leaves(count_leaves);
        vector<bool> traverse((count_leaves - 1) * 4);
        for (uint8_t &leaf : leaves) {
            b_in.read(&leaf, 1);
        }

        for (std::size_t i = 0; i < traverse.size(); i++) {
            bool bit;
            b_in >> bit; //big sad
            traverse[i] = bit;
        }

        HuffmanTree tree(leaves, traverse);

        BitStream::iBitStream_iterator iter(b_in);
        BitStream::iBitStream_iterator eof_iter;
        while (iter != eof_iter && count_symbols > 0) {
            auto symbol = tree.find(iter, eof_iter);
            b_out.write(&symbol, 1);
            count_symbols--;
        }

        b_out.close();
        b_in.close();

        int64_t header = 0;
        header += sizeof(count_symbols);
        header += sizeof(count_leaves);
        header += count_leaves;
        header += (count_leaves - 1) / 2;
        return header;
    }

    std::map<uint8_t, int64_t> HuffmanArchiver::get_frequency() {
        return std::map<uint8_t, int64_t>();
    }
}