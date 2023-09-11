#
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

#This makefile is used to test the other Makefiles

TOP = ../..
INCLUDE = include
LIB = lib

PROG = embedded_c
SRC = embedded_c.c

HTTP_LIB = libhttp.a

# If statically linking SSL you might need to:
#
#   * set the include path for the lib — e.g., `-I/opt/local/include/openssl-1.0/`
#   * set the library path for the bin — e.g., `-L/opt/local/lib/openssl-1.0/`

# Compiling with `-DNO_SSL_DL` doesn't currently work, and dynamically loaded libs
# take precedence over statically linked libs; on Mac you may need to:
#
#   * set the dynamic load path for the bin — e.g., `DYLD_LIBRARY_PATH=/opt/local/lib/openssl-1.0/ ./embedded_c`

# If you do those things SSL works, but given those things the default is `-DNO_SSL`.

# CFLAGS =          -DUSE_IPV6 -DUSE_SSL_DH -I$(TOP)/$(INCLUDE)
  CFLAGS = -DNO_SSL -DUSE_IPV6              -I$(TOP)/$(INCLUDE)

# DFLAGS='         -DUSE_STACK_SIZE=102400 -I...'
  DFLAGS='-DNO_SSL -DUSE_STACK_SIZE=102400      '

# LDFLAGS = -L...
  LDFLAGS =

# LIBS = -lcrypto -lpthread -lssl
  LIBS =          -lpthread

include $(TOP)/resources/Makefile.in-os

ifeq ($(TARGET_OS),LINUX)
	LIBS += -ldl
endif

all: $(PROG)

$(PROG): $(HTTP_LIB) $(SRC)
	$(CC) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(HTTP_LIB) $(LIBS)

$(HTTP_LIB):
	$(MAKE) -C $(TOP) DFLAGS=$(DFLAGS) clean $(LIB)/$(HTTP_LIB)
	cp $(TOP)/$(LIB)/$(HTTP_LIB) .

clean:
	rm -f $(HTTP_LIB) $(PROG)

.PHONY: all clean

