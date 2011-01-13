/*
    tudor-volumed
    ~~~~~~~~~~~~~

    Lightweight, desktop environment agnostic volume management daemon.

    :copyright: (c) 2010 David 'dav' Gidwani
    :license: New BSD License. See LICENSE for details.
*/
#include <iostream>
#include <X11/Xlib.h>
#include <X11/XF86keysym.h>
#include "environment.h"
#include "mixer.h"
#include "util.h"

#ifdef USE_OSS
#include "ossmixer.h"
#endif


#ifdef USE_ALSA
#include "alsamixer.h"
#endif


IMixer* setup_mixer(Environment* env)
{
#ifdef USE_OSS
    const std::string mixerdev = env->get_str("OSS_MIXERDEV", "/dev/mixer");
    if (!exists("/proc/asound"))
    {
        OssMixer* mixer = new OssMixer(mixerdev);
        return mixer;
    }
#endif
#ifdef USE_ALSA
    const std::string channel = env->get_str("alsa_channel");
    const std::string cardname = env->get_str("alsa_cardname", "default");
    AlsaMixer* mixer = new AlsaMixer(cardname);
    if (channel.empty())
        mixer->set_active_playback_elem();
    else
        mixer->set_active_playback_elem(channel);
    mixer->set_card_name(cardname);
    return mixer;
#endif
    fatal_error("recompile with USE_OSS or USE_ALSA");
}


int main(int argc, char *argv[])
{
    Environment* env = new Environment("TUDOR_VOLUMED");
    IMixer* mixer = setup_mixer(env);

    Window root;
    XEvent event;
    Display *dpy;

    dpy = XOpenDisplay(0x0);
    root = DefaultRootWindow(dpy);

    const int increment    = env->get_int("increment", 5, 5, 50);
    const KeyCode kc_raise = XKeysymToKeycode(dpy, XF86XK_AudioRaiseVolume);
    const KeyCode kc_lower = XKeysymToKeycode(dpy, XF86XK_AudioLowerVolume);
    const KeyCode kc_mute  = XKeysymToKeycode(dpy, XF86XK_AudioMute);

    XGrabKey(dpy, kc_raise, AnyModifier, root, True,
        GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, kc_lower, AnyModifier, root, True,
        GrabModeAsync, GrabModeAsync);
    XGrabKey(dpy, kc_mute, AnyModifier, root, True,
        GrabModeAsync, GrabModeAsync);

    while (true)
    {
        XNextEvent(dpy, &event);
        if (KeyPress == event.type)
        {
            unsigned int keycode = event.xkey.keycode;
            if (kc_raise == keycode)
                mixer->raise_volume(increment);
            else if (kc_lower == keycode)
                mixer->lower_volume(increment);
            else if (kc_mute == keycode)
                mixer->toggle_mute();
        }
    }
    return 0;
}
