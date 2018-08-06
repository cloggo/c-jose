#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <jose/jose.h>
#include <lib/hooks.h>

#include "module.h"

NAPI_METHOD(c_jose_jwk_thp_buf) {
  NAPI_METHOD_ARG(3);

  napi_status status;

  napi_valuetype resultType;

  size_t jwkLength = 0;
  char* jwk  = NULL;

  status = napi_typeof(env, argv[0], &resultType);
  assert(status == napi_ok);

  json_t *jwk_json  = NULL;

  if(resultType != napi_null) {
    status = napi_get_value_string_utf8(env, argv[0], NULL, 0, &jwkLength); \
    assert(status == napi_ok);
    jwk = malloc(++jwkLength);
    status = napi_get_value_string_utf8(env, argv[0], jwk, jwkLength, &jwkLength); \
    assert(status == napi_ok || free_char(&jwk));
    jwk_json = json_loads(jwk, 0, NULL);
    free_char(&jwk);
    assert(jwk_json);
  }

  JS_STRING_TO_C_CHAR(env, argv[1], alg, status);
  assert(algLength || free_json(&jwk_json));

  if(algLength) {
    assert(jose_hook_alg_find(JOSE_HOOK_ALG_KIND_HASH, alg) || free_json(&jwk_json));
  }

  void *thp = NULL;
  size_t thp_size = 0;

  status = napi_typeof(env, argv[2], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_arraybuffer_info(env, argv[2], &thp, &thp_size);
    assert(status == napi_ok || free_json(&jwk_json));
  }

  size_t dlen = jose_jwk_thp_buf(NULL, jwk_json, alg, (uint8_t *) thp, thp_size);

  free_json(&jwk_json);

  napi_value result;

  if(!thp) {
    status = napi_create_uint32(env, dlen, &result);
    assert(status == napi_ok);
    return result;
  }

  return argv[2];
}

