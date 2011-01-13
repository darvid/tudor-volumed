/*
    util
    ~~~~

    Assorted convenience routines. Part of *tudor-volumed*.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include "util.h"

bool exists(const std::string& path)
{
    struct stat st;
    return (stat(path.c_str(), &st) == 0 ? true : false);
}

int find_last_space_pos(std::string str)
{
    std::string::size_type pos;
    for (pos = str.find_last_of(' ');
         pos != std::string::npos;)
    {
        if (str.substr(pos - 1, 1) == "\\")
        {
            --pos;
            str = str.substr(0, pos);
            pos = str.find_last_of(' ');
        }
        break;
    }
    return pos;
}

std::string upper(const std::string& str)
{
    std::string newstr(str);
    std::transform(newstr.begin(), newstr.end(), newstr.begin(),
        ::toupper);
    return newstr;
}

void warning(const std::string& msg)
{
    std::cerr << "warning: " << msg << std::endl;
}

void fatal_error(const std::string& msg)
{
    std::cerr << msg << std::endl;
    exit(1);
}

#ifdef USE_ALSA
#include <alsa/asoundlib.h>

void fatal_alsa_error(const std::string msg, int alsa_err)
{
    std::cerr << msg << ": " << snd_strerror(alsa_err) << std::endl;
    exit(1);
}

#endif /* USE_ALSA */
