############################################################################
# apps/examples/coap/Make.defs
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
############################################################################

include $(APPDIR)/Make.defs

CFLAGS += ${shell $(INCDIR) "$(CC)" $(APPDIR)/include/netutils}
CFLAGS += -DCOAP_CONSTRAINED_STACK=0 -DBSD=0

# CoAP Server/Client built-in application info

PROGNAME  = coap_server coap_client
PRIORITY  = $(CONFIG_EXAMPLES_COAP_PRIORITY)
STACKSIZE = $(CONFIG_EXAMPLES_COAP_STACKSIZE)
MODULE    = $(CONFIG_EXAMPLES_COAP)

# CoAP Server/Client Example

CSRCS = common.c
MAINSRC = coap_server.c coap_client.c

include $(APPDIR)/Application.mk
