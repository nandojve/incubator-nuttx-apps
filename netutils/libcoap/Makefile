#############################################################################
# apps/netutils/libcoap/Makefile
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
#############################################################################

# ./configure --disable-shared --disable-fast-install --disable-documentation
# --disable-doxygen --disable-manpages --disable-dtls --disable-tests
# --disable-examples --disable-examples-source --disable-tcp --disable-async
# --without-gnutls --without-openssl --without-mbedtls --without-tinydtls
# --without-epoll

# Standard includes

include $(APPDIR)/Make.defs

# Set up build configuration and environment

WD := ${shell echo $(CURDIR) | sed -e 's/ /\\ /g'}

CONFIG_NETUTILS_LIBCOAP_URL ?= "https://github.com/obgm/libcoap/archive"
CONFIG_NETUTILS_LIBCOAP_VERSION ?= "4.3.0"
LIBCOAP_VERSION = $(patsubst "%",%,$(strip $(CONFIG_NETUTILS_LIBCOAP_VERSION)))

LIBCOAP_TARBALL = v$(LIBCOAP_VERSION).tar.gz

LIBCOAP_UNPACKNAME = libcoap-$(LIBCOAP_VERSION)

LIBCOAP_UNPACKDIR =  $(WD)$(DELIM)$(LIBCOAP_UNPACKNAME)
LIBCOAP_SRCDIR = $(LIBCOAP_UNPACKNAME)$(DELIM)src
LIBCOAP_INCDIR = $(LIBCOAP_UNPACKDIR)$(DELIM)include$(DELIM)coap3

APPS_INCDIR = $(APPDIR)$(DELIM)include$(DELIM)netutils
APPS_COAP_INCDIR = $(APPS_INCDIR)$(DELIM)coap3

libcoap_dir := $(filter %libcoap,$(APPDS))
vpath %c %h $(libcoap_dir)$(DELIM)src

# set include path for coap sources
CFLAGS += ${shell $(INCDIR) "$(CC)" $(APPS_INCDIR)}
CFLAGS += ${shell $(INCDIR) "$(CC)" $(APPS_COAP_INCDIR)}
CFLAGS += -DCOAP_CONSTRAINED_STACK=0 -DBSD=0

CSRCS =  $(LIBCOAP_SRCDIR)$(DELIM)address.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)async.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)block.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_cache.c
ifeq ($(CONFIG_NETUTILS_LIBCOAP_DEBUG),y)
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_debug.c
endif
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_hashkey.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_io.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_notls.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_session.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_tcp.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_time.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)coap_prng.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)encode.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)mem.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)net.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)option.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)pdu.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)resource.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)str.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)subscribe.c
CSRCS += $(LIBCOAP_SRCDIR)$(DELIM)uri.c

$(LIBCOAP_TARBALL):
	@echo "Downloading: $(LIBCOAP_TARBALL)"
	$(Q) curl -O -L $(CONFIG_NETUTILS_LIBCOAP_URL)$(DELIM)$(LIBCOAP_TARBALL)

$(LIBCOAP_UNPACKNAME): $(LIBCOAP_TARBALL)
	@echo "Unpacking: $(LIBCOAP_TARBALL) -> $(LIBCOAP_UNPACKNAME)"
	$(Q) tar xzf $(LIBCOAP_TARBALL)
	$(Q) rm $(LIBCOAP_INCDIR)$(DELIM)coap_riot.h
	$(Q) rm $(LIBCOAP_INCDIR)$(DELIM)coap.h.in
	$(Q) rm $(LIBCOAP_INCDIR)$(DELIM)coap.h.windows
	$(Q) rm $(LIBCOAP_INCDIR)$(DELIM)coap.h.windows.in
	$(Q) rm $(LIBCOAP_INCDIR)$(DELIM)lwippools.h
	$(Q) mkdir -p $(APPS_COAP_INCDIR)
	$(Q) cp $(WD)$(DELIM)coap_config.h $(APPS_COAP_INCDIR)
	$(Q) cp $(WD)$(DELIM)coap.h $(APPS_COAP_INCDIR)
	$(Q) cp $(LIBCOAP_INCDIR)$(DELIM)*.h $(APPS_COAP_INCDIR)
	$(Q) touch $(LIBCOAP_UNPACKNAME)

context:: $(LIBCOAP_UNPACKNAME)

distclean::
	$(call DELDIR, $(LIBCOAP_UNPACKNAME))
	$(call DELFILE, $(LIBCOAP_TARBALL))

include $(APPDIR)/Application.mk
