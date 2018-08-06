#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <jose/jose.h>
#include <lib/hooks.h>

#include "module.h"

NAPI_METHOD(c_jose_b64_dec_buf) {
  NAPI_METHOD_ARG(1);
  napi_status status;
  JS_STRING_TO_C_CHAR(env, argv[0], b64, status);
  size_t outLength = jose_b64_dec_buf(b64, b64Length, NULL, 0);
  char out[outLength];
  jose_b64_dec_buf(b64, b64Length, out, outLength);

  napi_value result;
  napi_create_string_utf8(env, out, outLength, &result);

  return result;
}

NAPI_METHOD(c_jose_b64_enc_sbuf) {
  NAPI_METHOD_ARG(1);
  napi_status status;
  JS_STRING_TO_C_CHAR(env, argv[0], sbuf, status);
  size_t outLength = jose_b64_enc_buf(sbuf, sbufLength, NULL, 0);
  char out[outLength];
  jose_b64_enc_buf(sbuf, sbufLength, out, outLength);

  napi_value result;
  napi_create_string_utf8(env, out, outLength, &result);

  return result;
}

NAPI_METHOD(c_jose_b64_enc_bbuf) {
  NAPI_METHOD_ARG(1);
  napi_status status;

  void *bbuf = NULL;
  size_t bbufLength = 0;

  status = napi_get_arraybuffer_info(env, argv[0], &bbuf, &bbufLength);
  assert(status == napi_ok);

  size_t outLength = jose_b64_enc_buf(bbuf, bbufLength, NULL, 0);
  char out[outLength];

  jose_b64_enc_buf(bbuf, bbufLength, out, outLength);

  napi_value result;
  napi_create_string_utf8(env, out, outLength, &result);

  return result;
}
