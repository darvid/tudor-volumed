/*
    environment
    ~~~~~~~~~~~

    Ugly but simple environment variable interface. Part of *tudor-volumed*.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#include <iostream>
#include <cstdlib>
#include "environment.h"
#include "util.h"

Environment::Environment(std::string prefix="")
{
    this->set_separator("_");
    this->set_prefix(prefix);
}

std::string Environment::get_prefix()
{
    return this->prefix;
}

void Environment::set_prefix(const std::string& prefix)
{
    this->prefix = prefix;
}

std::string Environment::get_separator()
{
    return this->separator;
}

void Environment::set_separator(const std::string& separator)
{
    this->separator = separator;
}

std::string Environment::expand_variable_name(const std::string& name)
{
    return (this->prefix + this->separator + upper(name));
}

std::string Environment::get_str(const std::string& variable_name,
                                 const std::string& default_value,
                                 const bool use_prefix)
{
    const char* value = getenv((use_prefix ?
        this->expand_variable_name(variable_name) : variable_name).c_str());
    return (value ? value : default_value);
}

int Environment::get_int(const std::string& variable_name,
                         const int& default_value,
                         const int& min, const int& max,
                         const bool use_prefix)
{
    const char* value = getenv((use_prefix ?
        this->expand_variable_name(variable_name) : variable_name).c_str());
    if (NULL == value) return default_value;
    const int int_value = atoi(value);
    if (int_value != 0 && max > 0 && min > 0 && max > min
        && max >= int_value && int_value >= min)
        return int_value;
    else
    {
        warning("invalid value for " + (use_prefix ?
            this->expand_variable_name(variable_name) : variable_name));
        return default_value;
    }
}
