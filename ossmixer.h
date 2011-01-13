/*
    tudor-volumed
    ~~~~~~~~~~~~~

    Lightweight, desktop environment agnostic volume management daemon.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#ifndef TUDOR_VOLUMED_OSSMIXER_H
#define TUDOR_VOLUMED_OSSMIXER_H
#include <soundcard.h>
#include "mixer.h"

class OssMixer: public IMixer
{
    protected:
        int handle;
        int dev_no;
        int num_ext;
        void find_channel(const std::string name, oss_mixer_value* value);
    public:
        OssMixer();
        OssMixer(const std::string mixer_device);
        virtual ~OssMixer();
        void clean_up();
        int  get_volume();
        void set_volume(int percentage);
        void raise_volume(int percentage);
        void lower_volume(int percentage);
        bool get_mute();
        void mute_volume();
        void unmute_volume();
        void toggle_mute();
};

#endif /* TUDOR_OSSMIXER_H */
