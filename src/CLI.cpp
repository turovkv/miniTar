#include <set>
#include <iostream>
#include "CLI.h"

namespace mini_tar {
    CLIException::CLIException(string_view message) :
            std::logic_error(message.data()) {}

    CLI::CLI(int argc, char *argv[]) {
        for (int i = 1; i < argc; i++) {
            string_view word = argv[i];
            if (word.empty()) {
                continue;
            }

            if (!is_parameter(word)) {
                if (!all_parameters_are_set()) {
                    throw CLIException("invalid parameters: anonymous args must be in the end: " +
                                       static_cast<std::string>(word));
                }
                anonymous_args_.emplace_back(word);
                continue;
            }

            if (!reduce_.count(word)) {
                throw CLIException("invalid parameters: parameter does not exist: " +
                                   static_cast<std::string>(word));
            }
            word = reduce_[word];
            int group = parameter_to_group_[word];

            if (group_value_.count(group)) {
                throw CLIException("invalid parameters: ambiguous parameter: " +
                                   static_cast<std::string>(word));
            }

            if (!group_has_value_[group]) {
                group_value_[group] = {word, ""};
                continue;
            }


            if (i + 1 < argc && !is_parameter(argv[i + 1])) {
                group_value_[group] = {word, argv[i + 1]};
                i = i + 1;
                continue;
            }

            throw CLIException("invalid parameters: parameter has no value: " +
                               static_cast<std::string>(word));
        }

        if (!all_parameters_are_set()) {
            throw CLIException("invalid parameters: not all parameters are set ");
        }

        if (anonymous_args_.size() != count_anonymous_args) {
            throw CLIException("invalid parameters: invalid number of anonymous args: expected " +
                               std::to_string(count_anonymous_args) +
                               " but found " +
                               std::to_string(anonymous_args_.size())
            );
        }
    }

    bool CLI::is_parameter(string_view word) {
        return word[0] == '-';
    }

    bool CLI::all_parameters_are_set() {
        bool ans = true;
        for (auto &[key, val] : is_optional_group_) {
            if (!val && !group_value_.count(key)) {
                ans = false;
            }
        }
        return ans;
    }

    string_view CLI::get_tar_mode() {
        return group_value_[tar_mode_group].first;
    }

    string_view CLI::get_dir_src() {
        if (group_value_.count(dir_src_group)) {
            return group_value_[dir_src_group].second;
        } else {
            return group_default_value_[dir_src_group];
        }
    }

    string_view CLI::get_dir_dest() {
        return anonymous_args_.front();
    }
}