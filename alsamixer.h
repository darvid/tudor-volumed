#ifndef TUDOR_ALSAMIXER_H
#define TUDOR_ALSAMIXER_H
#include <string>
#include <alsa/asoundlib.h>
#include "mixer.h"

#define UNKN  SND_MIXER_SCHN_UNKNOWN
#define LEFT  SND_MIXER_SCHN_FRONT_LEFT
#define RIGHT SND_MIXER_SCHN_FRONT_RIGHT

class AlsaMixer: public IMixer
{
    protected:
        snd_mixer_t      *handle;
        snd_mixer_elem_t *active_elem;
        std::string      cardname;
        long min;
        long max;
    private:
        int load_handle();
        snd_mixer_elem_t* get_playback_elem(const std::string name);
        snd_mixer_elem_t* get_first_playback_elem();
    public:
        AlsaMixer(const std::string& cardname);
        virtual ~AlsaMixer();
        void clean_up();
        int  get_volume();
        void set_volume(int percentage);
        void raise_volume(const int percentage);
        void lower_volume(const int percentage);
        bool get_mute();
        void mute_volume();
        void unmute_volume();
        void toggle_mute();
        void set_card_name(const std::string card_name);
        int set_active_playback_elem(snd_mixer_elem_t& elem);
        int set_active_playback_elem(const std::string name);
        int set_active_playback_elem();
};

#endif /* TUDOR_ALSAMIXER_H */
