/*
    mixer
    ~~~~~

    Primitive mixer interface. Part of *tudor-volumed*.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#include <string>
#include "mixer.h"

void IMixer::set_channel(std::string channel)
{
    this->channel = channel;
};

std::string IMixer::get_channel()
{
    return this->channel;
};
