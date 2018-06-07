SHELL = /bin/sh
#CC      = gcc
CC      = g++
AR      = ar
ARFL    = rv
RANLIB  = ranlib

CFLAGS = -c -static -g -fno-inline -W -Wall -Wcast-qual -Wcast-align \
-Wpointer-arith -O2 \
-D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -D_USE_FAST_MACRO \
-DHAS_LIB_CORE -DUSE_LIBCORE_LOG -std=c++11
###########################################################
#Check system:
#       Linux, SunOS, Solaris, BSD variants, AIX, HP-UX
SYSLIB =
CHECKSYSRES = @echo "Unknow system type!";exit 1
UNIXNAME = $(shell uname -sm)

#Path for Linux
ifeq ($(findstring Linux, $(UNIXNAME)), Linux)
	ifeq ($CC, "g++")
		CFLAGS += -Wstrict-prototypes
	endif
	ifeq ($(findstring i686, $(UNIXNAME)), i686)
		SYS_PATH=linux32
		GLIB_INCL_PATH = -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include
	endif
	ifeq ($(findstring x86_64, $(UNIXNAME)), x86_64)
		SYS_PATH=linux64
		GLIB_INCL_PATH = -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include
	endif
	CFLAGS += -DLINUX2
	SYSLIB = -lcrypt -lpthread
endif

#Path for SunOS
ifeq ($(findstring SunOS, $(UNIXNAME)), SunOS)
	ifeq ($(findstring 86, $(UNIXNAME)), 86)
		SYSLIB = -lsocket -lnsl -lrt
	endif
	ifeq ($(findstring sun4u, $(UNIXNAME)), sun4u)
		SYSLIB = -lsocket -lnsl -lrt
	endif
	ifeq ($CC, "gcc")
		CFLAGS += -Wstrict-prototypes
	endif
	SYS_PATH=sunos_x86
	CFLAGS += -DSUNOS5
endif

#Path for HP-UX
ifeq ($(findstring HP-UX, $(UNIXNAME)), HP-UX)
	ifeq ($CC, "gcc")
		CFLAGS += -Wstrict-prototypes
	endif
	SYSLIB = -lpthread
	CFLAGS += -DHP_UX -DHPUX11
	PLAT_NAME=hp-ux
endif

#Find system type.
ifneq ($(SYSPATH),)
	CHECKSYSRES = @echo "System is $(shell uname -sm)"
endif

ifeq ($(BTC), used)
	CFLAGS += -D BTC
endif
###########################################################

LIB_NAME_PATH = -L$(C8Y_LIB_PATH)/lib -L/usr/local/lib64  -L/usr/lib64

ALL_LIBS = -lcurl -llua5.1 -lm -ldl -lsera -lboost_system -lm -lboost_thread -pthread

INCLUDE =  -I. -I./include -I$(C8Y_LIB_PATH)/include 

CFLAGS += $(INCLUDE)

OUTPATH = ./build/
OBJ_OUTPATH = $(OUTPATH)

#Project's objs
SOURCES = $(wildcard ./src/*.cc)
INCLUDES = $(wildcard ./include/*.h)
OBJS = $(patsubst ./src/%.cc,$(OBJ_OUTPATH)%.o,$(SOURCES))

###########################################################

PROG_NAME  = ./bin/tcp_client

.PHONY = clean
COMPILE = $(CC) $(CFLAGS)

all: $(PROG_NAME) 
	@echo $(SOURCES)

$(PROG_NAME): $(OBJS)
	@echo "$(PROG_NAME)"
	$(CC) -o $(PROG_NAME) $(OBJS) $(LIB_NAME_PATH) $(ALL_LIBS)

$(OBJ_OUTPATH)%.o: ./src/%.cc ./include/*.h
	@echo "$(CC) $@"
	$(COMPILE) -o $@ $<

clean:
	rm -f $(OBJS) $(PROG_NAME)
	rm -f $(PROG_NAME)$(PROG_NAME)

rebuild: clean all
