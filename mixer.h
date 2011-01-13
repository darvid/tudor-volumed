/*
    mixer
    ~~~~~

    Primitive mixer interface. Part of *tudor-volumed*.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#ifndef TUDOR_VOLUMED_MIXER_H
#define TUDOR_VOLUMED_MIXER_H
#include <string>

class IMixer
{
    protected:
        std::string channel;
        long        volume;
        long        muted_volume;
    public:
        void set_channel(std::string channel);
        std::string get_channel();
        virtual void clean_up() = 0;
        virtual int get_volume() = 0;
        virtual void set_volume(int percentage) = 0;
        virtual void raise_volume(int percentage) = 0;
        virtual void lower_volume(int percentage) = 0;
        virtual void mute_volume() = 0;
        virtual void unmute_volume() = 0;
        virtual void toggle_mute() = 0;
};

#endif /* TUDOR_VOLUMED_MIXER_H */
