#pragma once

#include <bits/unique_ptr.h>
#include <vector>
#include <map>
#include <iostream>

namespace Huffman {
    using std::vector;

    class HuffmanException : public std::runtime_error {
    public:
        explicit HuffmanException(std::string_view message);
    };


    class HuffmanNode {
    public:
        explicit HuffmanNode(uint8_t _value = 0) noexcept;
        bool is_leaf() const noexcept;

        uint8_t value;
        HuffmanNode *parent;
        std::unique_ptr<HuffmanNode> left;
        std::unique_ptr<HuffmanNode> right;
    };

    class HuffmanTree {
    public:
        HuffmanTree() = delete;
        explicit HuffmanTree(const std::map<uint8_t, int64_t> &frequency);
        HuffmanTree(const vector<uint8_t> &leaves, const vector<bool> &traverse);

        std::map<uint8_t, vector<bool>> get_all_codes() const;
        std::pair<vector<uint8_t>, vector<bool>> get_tree_info() const; // leaves and traverse
        template<typename Iterator>
        uint8_t find(Iterator &iter, Iterator eof_iter) const; //symbol

    private:
        void dfs_codes(HuffmanNode *vertex,
                       vector<bool> &cur_code,
                       std::map<uint8_t, vector<bool>> &codes) const;
        void dfs_tree_info(HuffmanNode *vertex,
                           vector<uint8_t> &leaves,
                           vector<bool> &traverse) const;

    private:
        std::unique_ptr<HuffmanNode> root;
    };

    template<typename Iterator>
    uint8_t HuffmanTree::find(Iterator &iter, Iterator eof_iter) const {
        HuffmanNode *cur_node = root.get();
        while (!cur_node->is_leaf()) {
            HuffmanNode *left_son = cur_node->left.get();
            HuffmanNode *right_son = cur_node->right.get();
            if (iter == eof_iter) {
                throw HuffmanException("HuffmanTree::find(...): unexpected EOF");
            }
            if (*iter == 0) {
                if (!left_son) {
                    throw HuffmanException("HuffmanTree::find(...): incorrect code");
                }
                cur_node = left_son;
            } else if (*iter == 1) {
                if (!right_son) {
                    throw HuffmanException("HuffmanTree::find(...): incorrect code");
                }
                cur_node = right_son;
            } else {
                throw HuffmanException("HuffmanTree::find(...): incorrect args");
            }
            ++iter;
        }

        return cur_node->value;
    }
}
