#pragma once

#include <vector>
#include <unordered_map>

namespace mini_tar {
    using std::string_view;

    class CLIException : public std::logic_error {
    public:
        explicit CLIException(string_view message);
    };

    class CLI {
    public:
        static constexpr string_view CREATE = "-c";
        static constexpr string_view EXTRACT = "-x";
        static constexpr int tar_mode_group = 0;

        static constexpr string_view DIRECTORY = "-d";
        static constexpr int dir_src_group = 1;

        static constexpr int count_anonymous_args = 1;

    public:
        CLI(int argc, char *argv[]);

        string_view get_tar_mode();

        string_view get_dir_src();

        string_view get_dir_dest();

    private:
        static bool is_parameter(string_view word);

        bool all_parameters_are_set();

    private:
        std::unordered_map<string_view, string_view> reduce_ = {
                {CREATE,        CREATE},
                {EXTRACT,       EXTRACT},
                {DIRECTORY,     DIRECTORY},
                {"--extract",    CREATE},
                {"--extract",   EXTRACT},
                {"--directory", DIRECTORY},
        };
        std::unordered_map<string_view, int> parameter_to_group_ = {
                {CREATE,    tar_mode_group},
                {EXTRACT,   tar_mode_group},
                {DIRECTORY, dir_src_group},
        };
        std::unordered_map<int, bool> is_optional_group_ = {
                {tar_mode_group, false},
                {dir_src_group,  true},
        };
        std::unordered_map<int, bool> group_has_value_ = {
                {tar_mode_group, false},
                {dir_src_group,  true},
        };
        std::unordered_map<int, string_view> group_default_value_ = {
                {tar_mode_group, ""},
                {dir_src_group,  "."},
        };

        std::unordered_map<int, std::pair<string_view, string_view>> group_value_;
        std::vector<string_view> anonymous_args_;
    };
}