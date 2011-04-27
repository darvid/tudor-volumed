#include <iostream>
#include "alsamixer.h"
#include "mixer.h"
#include "util.h"

AlsaMixer::AlsaMixer(const std::string& cardname) : IMixer()
{
    int err;
    this->volume = -1;
    this->muted_volume = 100;
    this->set_card_name(cardname);
    if (0 > (err = this->load_handle()))
    {
        this->clean_up();
        fatal_alsa_error("unable to open mixer", err);
    }
}

AlsaMixer::~AlsaMixer()
{
    this->clean_up();
}

void AlsaMixer::clean_up()
{
    if (this->handle)
        snd_mixer_close(this->handle);
}

int AlsaMixer::load_handle()
{
    int err;
    if (((err = snd_mixer_open(&this->handle, 0)) < 0)
        || ((err = snd_mixer_attach(this->handle, this->cardname.c_str())) < 0)
        || ((err = snd_mixer_selem_register(this->handle, NULL, NULL)) < 0)
        || ((err = snd_mixer_load(this->handle)) < 0))
        return err;
    return 0;
}

void AlsaMixer::set_card_name(const std::string card_name)
{
    this->cardname = card_name;
}

snd_mixer_elem_t* AlsaMixer::get_playback_elem(const std::string name)
{
    snd_mixer_elem_t* elem;
    for (elem = snd_mixer_first_elem(this->handle);
         elem;
         elem = snd_mixer_elem_next(elem))
    {
        if (snd_mixer_selem_is_active(elem) &&
            snd_mixer_selem_has_playback_volume(elem) &&
            (name == snd_mixer_selem_get_name(elem)))
            return elem;
    }
}

snd_mixer_elem_t* AlsaMixer::get_first_playback_elem()
{
    snd_mixer_elem_t* elem;
    for (elem = snd_mixer_first_elem(this->handle);
         elem;
         elem = snd_mixer_elem_next(elem))
    {
        if (snd_mixer_selem_is_active(elem) &&
            snd_mixer_selem_has_playback_volume(elem))
            return elem;
    }
}

int AlsaMixer::set_active_playback_elem(snd_mixer_elem_t& elem)
{
    this->active_elem = &elem;
    snd_mixer_selem_get_playback_volume_range(&elem, &this->min, &this->max);
    return 0;
}

int AlsaMixer::set_active_playback_elem(const std::string name)
{
    return this->set_active_playback_elem(*this->get_playback_elem(name));
}

int AlsaMixer::set_active_playback_elem()
{
    return this->set_active_playback_elem(*this->get_first_playback_elem());
}

int AlsaMixer::get_volume()
{
    int value;
    long left, right;
    snd_mixer_handle_events(this->handle);
    snd_mixer_selem_get_playback_volume(this->active_elem, LEFT, &left);
    snd_mixer_selem_get_playback_volume(this->active_elem, RIGHT, &right);
    value = (int)(((left + (right - left)/2)*100) / (this->max - this->min));
    return value;
}

void AlsaMixer::set_volume(int percentage)
{
    percentage = (percentage < 0) ? 0 : (percentage > 100 ? 100 : percentage);
    if (percentage == 0)
        volume = min;
    else if (percentage == 100)
        volume = max;
    else
        volume = min + ((this->max - this->min) * (long)percentage)/100;
    snd_mixer_selem_set_playback_volume(this->active_elem, LEFT, volume);
    snd_mixer_selem_set_playback_volume(this->active_elem, RIGHT, volume);
}

void AlsaMixer::raise_volume(const int percentage)
{
    this->set_volume(this->get_volume() + percentage);
}

void AlsaMixer::lower_volume(int percentage)
{
    this->set_volume(this->get_volume() - percentage);
}

bool AlsaMixer::get_mute()
{
    if (snd_mixer_selem_has_playback_switch(this->active_elem))
    {
        int muted;
        snd_mixer_handle_events(this->handle);
        snd_mixer_selem_get_playback_switch(this->active_elem, LEFT, &muted);
        snd_mixer_selem_get_playback_switch(this->active_elem, RIGHT, &muted);
        return !(bool)muted;
    }
    else
    {
        return (this->get_volume() == 0 ? true : false);
    }
}

void AlsaMixer::mute_volume()
{
    if (!snd_mixer_selem_has_playback_switch(this->active_elem))
    {
        this->muted_volume = this->get_volume();
        this->set_volume(0);
    }
    else
    {
        snd_mixer_handle_events(this->handle);
        snd_mixer_selem_set_playback_switch(this->active_elem, UNKN, 0);
    }
}

void AlsaMixer::unmute_volume()
{
    if (!snd_mixer_selem_has_playback_switch(this->active_elem))
    {
        std::cout << this->muted_volume << std::endl;
        this->set_volume(this->muted_volume);
    }
    else
    {
        snd_mixer_handle_events(this->handle);
        snd_mixer_selem_set_playback_switch(this->active_elem, UNKN, 1);
    }
}

void AlsaMixer::toggle_mute()
{
    this->get_mute() ? this->unmute_volume() : this->mute_volume();
}
