#include "doctest.h"
#include "Huffman.h"

using namespace Huffman;

TEST_CASE("HuffmanNode") {
    SUBCASE("construct 1") {
        HuffmanNode n;
        CHECK(n.value == 0);
        CHECK(n.parent == nullptr);
        CHECK((!n.left && !n.left));
    }
    SUBCASE("construct 2") {
        HuffmanNode n(228);
        CHECK(n.value == 228);
        CHECK(n.parent == nullptr);
        CHECK((!n.left && !n.left));
    }
    SUBCASE("is_leaf 1") {
        HuffmanNode n;
        CHECK(n.is_leaf());
    }
    SUBCASE("is_leaf 2") {
        HuffmanNode n;
        n.right = std::make_unique<HuffmanNode>(5);
        CHECK(!n.is_leaf());
    }

}

//HuffmanTree
TEST_CASE("construct(frequency) + get_tree_info") {
    SUBCASE ("2 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        Huffman::HuffmanTree tree(frequency);
        auto [leaves, traverse] = tree.get_tree_info();
        CHECK(leaves == vector<uint8_t>{1, 2});
        CHECK(traverse == vector<bool>{1, 0, 1, 0});
    }
    SUBCASE ("4 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        Huffman::HuffmanTree tree(frequency);
        auto [leaves, traverse] = tree.get_tree_info();
        CHECK(leaves == vector<uint8_t>{1, 2, 3, 4});
        CHECK(traverse == vector<bool>{1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0});
    }
    SUBCASE ("5 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        frequency[5] = 16;
        Huffman::HuffmanTree tree(frequency);
        auto [leaves, traverse] = tree.get_tree_info();
        CHECK(leaves == vector<uint8_t>{1, 2, 3, 4, 5});
        CHECK(traverse == vector<bool>{1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0});
    }
}

TEST_CASE("construct(frequency) + get_all_codes") {
    SUBCASE ("4 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        Huffman::HuffmanTree tree(frequency);
        auto codes = tree.get_all_codes();
        CHECK(codes[1] == vector<bool>{0, 0, 0});
        CHECK(codes[2] == vector<bool>{0, 0, 1});
        CHECK(codes[3] == vector<bool>{0, 1});
        CHECK(codes[4] == vector<bool>{1});
    }
    SUBCASE ("2 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        Huffman::HuffmanTree tree(frequency);
        auto codes = tree.get_all_codes();
        CHECK(codes[1] == vector<bool>{0});
        CHECK(codes[2] == vector<bool>{1});
    }
}

TEST_CASE("construct(leaves + traverse) + get_tree_info") {
    SUBCASE("4 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        Huffman::HuffmanTree tree(frequency);
        auto[leaves, traverse] = tree.get_tree_info();
        Huffman::HuffmanTree tree2(leaves, traverse);
        auto[leaves2, traverse2] = tree2.get_tree_info();

        CHECK(leaves == leaves2);
        CHECK(traverse == traverse2);
    }
}

TEST_CASE("construct(leaves + traverse) + get_all_codes") {
    SUBCASE("4 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        Huffman::HuffmanTree tree(frequency);
        auto[leaves, traverse] = tree.get_tree_info();
        Huffman::HuffmanTree tree2(leaves, traverse);

        auto codes = tree2.get_all_codes();
        CHECK(codes[1] == vector<bool>{0, 0, 0});
        CHECK(codes[2] == vector<bool>{0, 0, 1});
        CHECK(codes[3] == vector<bool>{0, 1});
        CHECK(codes[4] == vector<bool>{1});
    }
}


TEST_CASE("construct(frequency) + find" ) {
    SUBCASE ("4 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        Huffman::HuffmanTree tree(frequency);
        auto codes = tree.get_all_codes();

        for (int i = 1; i <= 4; i++) {
            auto it_beg = codes[i].begin();
            auto it_end = codes[i].end();
            CHECK(i == tree.find(it_beg, it_end));
        }
    }
    SUBCASE ("5 symbols") {
        std::map<uint8_t, int64_t> frequency;
        frequency[1] = 1;
        frequency[2] = 2;
        frequency[3] = 4;
        frequency[4] = 8;
        frequency[5] = 16;
        Huffman::HuffmanTree tree(frequency);
        auto codes = tree.get_all_codes();

        for (int i = 1; i <= 5; i++) {
            auto it_beg = codes[i].begin();
            auto it_end = codes[i].end();
            CHECK(i == tree.find(it_beg, it_end));
        }
    }
}
