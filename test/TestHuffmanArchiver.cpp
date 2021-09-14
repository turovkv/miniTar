#include <sstream>
#include "doctest.h"
#include "HuffmanArchiver.h"

using std::stringstream;

TEST_CASE("compress + extract") {
    SUBCASE("random") {
        constexpr int max_size = 1e4;
        constexpr int count_tests = 40;
        for (int i = 0; i < count_tests; i++) {
            stringstream file_orig;
            stringstream file_comp;
            stringstream file_uncomp;
            int size = rand() % max_size;
            for (int j = 0; j < size; j++) {
                file_orig.put(rand() % 256);
            }
            {
                HuffmanArchiver::HuffmanArchiver().compress(file_orig, file_comp);
            }
            {
                HuffmanArchiver::HuffmanArchiver().extract(file_comp, file_uncomp);
            }
            CHECK(file_orig.str() == file_uncomp.str());
        }
    }
    SUBCASE("0 byte") {
        stringstream file_orig;
        stringstream file_comp;
        stringstream file_uncomp;
        {
            HuffmanArchiver::HuffmanArchiver().compress(file_orig, file_comp);
        }
        {
            HuffmanArchiver::HuffmanArchiver().extract(file_comp, file_uncomp);
        }
        CHECK(file_orig.str() == file_uncomp.str());
    }
    SUBCASE("1 byte") {
        stringstream file_orig;
        stringstream file_comp;
        stringstream file_uncomp;
        file_orig.put(33);
        {
            HuffmanArchiver::HuffmanArchiver().compress(file_orig, file_comp);
        }
        {
            HuffmanArchiver::HuffmanArchiver().extract(file_comp, file_uncomp);
        }
        CHECK(file_orig.str() == file_uncomp.str());
    }
}