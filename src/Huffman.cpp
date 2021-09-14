//
// Created by kirill on 12/04/2020.
//

#include <set>
#include "Huffman.h"

namespace Huffman {
    HuffmanException::HuffmanException(std::string_view message) :
            std::runtime_error(message.data()) {}



    HuffmanNode::HuffmanNode(uint8_t _value) noexcept:
            value(_value),
            parent(),
            left(),
            right() {}

    bool HuffmanNode::is_leaf() const noexcept {
        return !left && !right;
    }

    HuffmanTree::HuffmanTree(const std::map<uint8_t, int64_t> &frequency) {
        if (frequency.empty()) {
            throw HuffmanException("HuffmanTree(...): empty frequency vector");
        }

        std::set<std::pair<int64_t, std::unique_ptr<HuffmanNode>>> nodes;
        //extract is the only way to take a move-only object out of a set
        auto extract_min = [](auto &container_set) {
            return move(container_set.extract(container_set.begin()).value());
        };

        for (const auto &[symbol, freq] : frequency) {
            nodes.emplace(freq, std::make_unique<HuffmanNode>(symbol));
        }
        while (nodes.size() > 1) {
            auto node_1 = extract_min(nodes);
            auto node_2 = extract_min(nodes);
            auto new_node = std::make_unique<HuffmanNode>();
            new_node->left = move(node_1.second);
            new_node->right = move(node_2.second);
            new_node->left->parent = new_node.get();
            new_node->right->parent = new_node.get();
            nodes.emplace(node_1.first + node_2.first, move(new_node));
        }

        root = extract_min(nodes).second;
    }

    HuffmanTree::HuffmanTree(const vector<uint8_t> &leaves, const vector<bool> &traverse) {
        if (traverse.empty() ||
            leaves.empty() ||
            traverse.size() != (leaves.size() - 1) * 4) {
            throw HuffmanException("HuffmanTree(...): invalid args");
        }

        auto leaves_iter = leaves.begin();
        root = std::make_unique<HuffmanNode>();
        HuffmanNode *current_node = root.get();
        for (auto event : traverse) {
            if (event == 1) { // down
                if (!current_node->left) {
                    current_node->left = std::make_unique<HuffmanNode>();
                    current_node->left->parent = current_node;
                    current_node = current_node->left.get();
                } else if (!current_node->right) {
                    current_node->right = std::make_unique<HuffmanNode>();
                    current_node->right->parent = current_node;
                    current_node = current_node->right.get();
                } else {
                    throw HuffmanException("HuffmanTree(...): incorrect traverse");
                }
            }
            if (event == 0) { // up
                if (current_node->is_leaf()) {
                    if (leaves_iter == leaves.end()) {
                        throw HuffmanException("HuffmanTree(...): incorrect traverse");
                    }
                    current_node->value = *leaves_iter++;
                }
                if (!current_node->parent) {
                    throw HuffmanException("HuffmanTree(...): incorrect traverse");
                }
                current_node = current_node->parent;
            }
        }
    }

    void HuffmanTree::dfs_codes(HuffmanNode *vertex,
                                vector<bool> &cur_code,
                                std::map<uint8_t, vector<bool>> &codes) const {
        HuffmanNode *left_son = vertex->left.get();
        HuffmanNode *right_son = vertex->right.get();
        if (vertex->is_leaf()) {
            codes[vertex->value] = cur_code;
        }
        if (left_son) {
            cur_code.emplace_back(0);
            dfs_codes(left_son, cur_code, codes);
            cur_code.pop_back();
        }
        if (right_son) {
            cur_code.emplace_back(1);
            dfs_codes(right_son, cur_code, codes);
            cur_code.pop_back();
        }
    }

    std::map<uint8_t, vector<bool>> HuffmanTree::get_all_codes() const {
        std::map<uint8_t, vector<bool>> codes;
        vector<bool> cur_code;

        dfs_codes(root.get(), cur_code, codes);

        return codes;
    }

    void HuffmanTree::dfs_tree_info(HuffmanNode *vertex, vector<uint8_t> &leaves,
                                    vector<bool> &traverse) const {
        HuffmanNode *left_son = vertex->left.get();
        HuffmanNode *right_son = vertex->right.get();
        if (left_son) {
            traverse.emplace_back(1);
            dfs_tree_info(left_son, leaves, traverse);
            traverse.emplace_back(0);
        }
        if (right_son) {
            traverse.emplace_back(1);
            dfs_tree_info(right_son, leaves, traverse);
            traverse.emplace_back(0);
        }
        if (vertex->is_leaf()) {
            leaves.emplace_back(vertex->value);
        }
    }

    std::pair<vector<uint8_t>, vector<bool>> HuffmanTree::get_tree_info() const {
        vector<uint8_t> leaves;
        vector<bool> traverse;

        dfs_tree_info(root.get(), leaves, traverse);

        return {leaves, traverse};
    }
}