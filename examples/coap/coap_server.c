/* minimal CoAP server
 *
 * Copyright (C) 2018-2021 Olaf Bergmann <bergmann@tzi.org>
 */

#include <nuttx/config.h>

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <crc32.h>

#include "common.h"

static void coap_method_handler_get(coap_resource_t *resource,
	coap_session_t *session, const coap_pdu_t * request,
	const coap_string_t * query, coap_pdu_t * response)
{
	coap_show_pdu(LOG_WARNING, request);
	coap_pdu_set_code(response, COAP_RESPONSE_CODE_CONTENT);
	coap_add_data(response, 5, (const uint8_t *)"world");
	coap_show_pdu(LOG_WARNING, response);
}

int main(int argc, FAR char *argv[])
{
  coap_context_t  *ctx = NULL;
  coap_address_t dst;
  coap_resource_t *resource = NULL;
  coap_endpoint_t *endpoint = NULL;
  int result = EXIT_FAILURE;;
  coap_str_const_t *ruri = coap_make_str_const("hello");
  coap_startup();

  /* resolve destination address where server should be sent */
  if (resolve_address("localhost", "5683", &dst) < 0) {
    /*coap_log(LOG_CRIT, "failed to resolve address\n");*/
    goto finish;
  }

  /* create CoAP context and a client session */
  ctx = coap_new_context(NULL);

  if (!ctx || !(endpoint = coap_new_endpoint(ctx, &dst, COAP_PROTO_UDP))) {
    /*coap_log(LOG_EMERG, "cannot initialize context\n");*/
    goto finish;
  }

  resource = coap_resource_init(ruri, 0);
  coap_register_handler(resource, COAP_REQUEST_GET, coap_method_handler_get);
  coap_add_resource(ctx, resource);

  while (true) { coap_io_process(ctx, COAP_IO_WAIT); }

  result = EXIT_SUCCESS;
 finish:

  coap_free_context(ctx);
  coap_cleanup();

  return result;
}
