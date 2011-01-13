#include <cstdio>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ossmixer.h"
#include "util.h"

OssMixer::OssMixer(const std::string mixer_device) : IMixer()
{
    this->dev_no = 0;
    if (-1 == (this->handle = open(mixer_device.c_str(), O_RDWR, 0)))
    {
        perror(mixer_device.c_str());
        exit(-1);
    }
    if (-1 == ioctl(this->handle, SNDCTL_MIX_NREXT, &this->num_ext))
    {
        perror("SND_MIX_NREXT");
        exit(-1);
    }
    fprintf(stdout, "%d\n", this->num_ext);
}


OssMixer::~OssMixer()
{
    this->clean_up();
}

void OssMixer::clean_up()
{
    if (this->handle)
        close(this->handle);
}

void OssMixer::find_channel(const std::string name, oss_mixer_value* value)
{
    oss_mixext ext;
    bool marker;
    for (int i = 0; i < this->num_ext; i++)
    {
        oss_mixer_value val;
        ext.dev = this->dev_no;
        ext.ctrl = i;
        if (-1 == ioctl(this->handle, SNDCTL_MIX_EXTINFO, &ext))
        {
            perror ("SNDCTL_MIX_EXTINFO");
            return;
        }
        if (MIXT_MARKER == ext.type)
        {
            marker = true;
            continue;
        }
        else if (!marker || 0 != strcmp(ext.extname, name.c_str()))
            continue;
        if (!(ext.flags & MIXF_WRITEABLE))
            return;
        fprintf(stdout, "%d\n", ext.type);

        val.dev = this->dev_no;
        val.ctrl = i;
        val.value = 0;
        val.timestamp = ext.timestamp;
        *value = val;
    }
}

int OssMixer::get_volume()
{
    oss_mixer_value val;
    this->find_channel("vmix0-outvol", &val);
    if (ioctl (this->handle, SNDCTL_MIX_READ, &val) == -1)
        perror("SNDCTL_MIX_READ");
    fprintf(stdout, "%d\n", val.value);
    int mask;
    if (-1 == ioctl(this->handle, SOUND_MIXER_READ_VOLUME, &mask))
        perror("read error");
    return mask;
}

void OssMixer::set_volume(int percentage)
{
    percentage = (percentage < 0) ? 0 : (percentage > 100 ? 100 : percentage);
    percentage = (percentage << 8) | (percentage & 0xFF);
    // if (-1 == ioctl(this->handle, MIXER_WRITE(4), &percentage))
    //     perror("write error");
}

void OssMixer::raise_volume(const int percentage)
{
    this->set_volume(this->get_volume() + percentage);
}

void OssMixer::lower_volume(int percentage)
{
    this->set_volume(this->get_volume() - percentage);
}

void OssMixer::mute_volume()
{

}

void OssMixer::unmute_volume()
{

}

void OssMixer::toggle_mute()
{

}