/* minimal CoAP client
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
#include <sys/socket.h>
#include <fcntl.h>
#include <netdb.h>

#include "common.h"

static coap_response_t coap_response_handler_get(coap_session_t *session,
	const coap_pdu_t *sent, const coap_pdu_t *received,
	const coap_mid_t mid)
{
	coap_show_pdu(LOG_WARNING, received);
	return COAP_RESPONSE_OK;
}

int main(int argc, FAR char *argv[])
{
  coap_context_t  *ctx = NULL;
  coap_session_t *session = NULL;
  coap_address_t dst;
  coap_pdu_t *pdu = NULL;
  int result = EXIT_FAILURE;;

  coap_startup();

  /* resolve destination address where server should be sent */
  if (resolve_address("coap.me", "5683", &dst) < 0) {
    /*coap_log(LOG_CRIT, "failed to resolve address\n");*/
    goto finish;
  }

  /* create CoAP context and a client session */
  ctx = coap_new_context(NULL);

  if (!ctx || !(session = coap_new_client_session(ctx, NULL, &dst,
                                                  COAP_PROTO_UDP))) {
    /*coap_log(LOG_EMERG, "cannot create client session\n");*/
    goto finish;
  }

  /* coap_register_response_handler(ctx, response_handler); */
  coap_register_response_handler(ctx, coap_response_handler_get);
  /* construct CoAP message */
  pdu = coap_pdu_init(COAP_MESSAGE_CON,
                      COAP_REQUEST_CODE_GET,
                      coap_new_message_id(session),
                      coap_session_max_pdu_size(session));
  if (!pdu) {
    /*coap_log( LOG_EMERG, "cannot create PDU\n" );*/
    goto finish;
  }

  /* add a Uri-Path option */
  coap_add_option(pdu, COAP_OPTION_URI_PATH, 5, (const uint8_t *)"hello");

  coap_show_pdu(LOG_WARNING, pdu);
  /* and send the PDU */
  coap_send(session, pdu);

  coap_io_process(ctx, COAP_IO_WAIT);

  result = EXIT_SUCCESS;
 finish:

  coap_session_release(session);
  coap_free_context(ctx);
  coap_cleanup();

  return result;
}
