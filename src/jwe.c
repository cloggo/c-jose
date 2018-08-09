#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <jose/jose.h>
#include <lib/hooks.h>

#include "module.h"


NAPI_METHOD(c_jose_jwe_hdr) {
  NAPI_METHOD_ARG(2);

  napi_status status;

  napi_valuetype resultType;

  void* jwe = NULL;
  void* rcp = NULL;

  status = napi_get_value_external(env, argv[0], &jwe);
  assert(status == napi_ok);

  status = napi_typeof(env, argv[1], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_value_external(env, argv[1], &rcp);
    assert(status == napi_ok);
  }

  json_t *out = jose_jwe_hdr((json_t*)jwe, (json_t*)rcp);

  napi_value result;

  status = napi_create_external(env, out, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;

}

NAPI_METHOD(c_jose_jwe_enc) {
  NAPI_METHOD_ARG(4);

  napi_status status;

  napi_valuetype resultType;

  void* jwe = NULL;
  void* rcp = NULL;
  void* jwk = NULL;

  status = napi_get_value_external(env, argv[0], &jwe);
  assert(status == napi_ok);

  status = napi_typeof(env, argv[1], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_value_external(env, argv[1], &rcp);
    assert(status == napi_ok);
  }

  status = napi_get_value_external(env, argv[2], &jwk);
  assert(status == napi_ok);

  void *pt = NULL;
  size_t ptLength = 0;

  status = napi_get_buffer_info(env, argv[2], &pt, &ptLength);
  assert(status == napi_ok);

  bool out = jose_jwe_enc(NULL, (json_t*)jwe, (json_t*)rcp, (json_t *)jwk, pt, ptLength);
  napi_value result;

  status = napi_get_boolean(env, out, &result);
  assert(status == napi_ok);
  return result;

}


NAPI_METHOD(c_jose_jwe_dec) {
  NAPI_METHOD_ARG(4);

  napi_status status;

  napi_valuetype resultType;

  void* jwe = NULL;
  void* rcp = NULL;
  void* jwk = NULL;

  status = napi_get_value_external(env, argv[0], &jwe);
  assert(status == napi_ok);

  status = napi_typeof(env, argv[1], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_value_external(env, argv[1], &rcp);
    assert(status == napi_ok);
  }

  status = napi_get_value_external(env, argv[2], &jwk);
  assert(status == napi_ok);

  size_t ptLength = 0;

  void* out = jose_jwe_dec(NULL, (json_t*)jwe, (json_t*)rcp, (json_t *)jwk, &ptLength);

  napi_value result;

  if(!out) {
    status = napi_get_boolean(env, out, &result);
    assert(status == napi_ok);
    return result;
  }

  status = napi_create_buffer(env, ptLength, &out, &result);

  return result;

}
