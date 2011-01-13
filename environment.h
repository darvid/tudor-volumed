/*
    environment
    ~~~~~~~~~~~

    Ugly but simple environment variable interface. Part of *tudor-volumed*.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#ifndef TUDOR_ENVIRONMENT_H
#define TUDOR_ENVIRONMENT_H
#include <string>

class Environment
{
    private:
        std::string prefix;
        std::string separator;
        std::string expand_variable_name(const std::string& name);
    public:
        explicit Environment(std::string prefix);
        std::string get_prefix();
        void set_prefix(const std::string& prefix);
        std::string get_separator();
        void set_separator(const std::string& separator);
        std::string get_str(const std::string& variable_name,
                            const std::string& default_value = "",
                            const bool use_prefix = true);
        int get_int(const std::string& variable_name,
                    const int& default_value,
                    const int& min=0, const int& max=0,
                    const bool use_prefix = true);
};

#endif /* TUDOR_VOLUMED_ENVIRONMENT_H */
