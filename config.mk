-include /etc/oss.conf

NAME := tudor-volumed
OS   := $(shell uname -o)
CXX  := gcc

LIBS    := -lX11 -lstdc++
BIN     := $(NAME)
OBJECTS := environment.o mixer.o util.o $(NAME).o

ifeq ($(OS), GNU/Linux)
	LIBS     += -lasound
	CXXFLAGS += -DUSE_ALSA
	OBJECTS  += alsamixer.o
endif

ifdef OSSLIBDIR
	CXXFLAGS += -DUSE_OSS -I${OSSLIBDIR}/include/sys
	OBJECTS  += ossmixer.o
endif