#ifndef TUDOR_VOLUMED_UTIL_H
#define TUDOR_VOLUMED_UTIL_H
#include <cstdlib>
#include <string>

bool exists(const std::string path);
int find_last_space_pos(std::string str);
std::string upper(const std::string& str);
void warning(const std::string& msg);
void fatal_error(const std::string& msg);

#ifdef USE_ALSA
#include <alsa/asoundlib.h>

void fatal_alsa_error(const std::string msg, int alsa_err);
#endif /* USE_ALSA */
#endif /* TUDOR_VOLUMED_UTIL_H */
