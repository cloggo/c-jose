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

  void* jwk  = NULL;

  status = napi_typeof(env, argv[0], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_value_external(env, argv[0], &jwk);
  }

  JS_STRING_TO_C_CHAR(env, argv[1], alg, status);
  assert(algLength);

  if(algLength) {
    assert(jose_hook_alg_find(JOSE_HOOK_ALG_KIND_HASH, alg));
  }

  void *thp = NULL;
  size_t thp_size = 0;

  status = napi_typeof(env, argv[2], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_buffer_info(env, argv[2], &thp, &thp_size);
    assert(status == napi_ok);
  }

  size_t dlen = jose_jwk_thp_buf(NULL, (json_t*)jwk, alg, (uint8_t *) thp, thp_size);

  assert(dlen != SIZE_MAX);

  napi_value result;

  status = napi_create_uint32(env, dlen, &result);
  assert(status == napi_ok);
  return result;

}

NAPI_METHOD(c_jose_jwk_gen) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  bool out = jose_jwk_gen(NULL, (json_t *)json);

  napi_value result;

  status = napi_get_boolean(env, out, &result);
  assert(status == napi_ok);
  return result;
}

NAPI_METHOD(c_jose_jwk_exc) {
  NAPI_METHOD_ARG(2);

  napi_status status;

  void *lcl;
  status = napi_get_value_external(env, argv[0], &lcl);
  assert(status == napi_ok);

  void *rem;
  status = napi_get_value_external(env, argv[1], &rem);
  assert(status == napi_ok);

  json_t* out = jose_jwk_exc(NULL, (json_t *)lcl, (json_t*)rem);

  napi_value result;

  if(!out) {
    status = napi_get_null(env, &result);
    assert(status == napi_ok);
    return result;
  }

  status = napi_create_external(env, out, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_jwk_pub) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  bool out = jose_jwk_pub(NULL, (json_t *)json);

  napi_value result;

  status = napi_get_boolean(env, out, &result);
  assert(status == napi_ok);
  return result;
}


NAPI_METHOD(c_jose_jwk_prm) {
  NAPI_METHOD_ARG(3);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  bool req;
  status = napi_get_value_bool(env, argv[1], &req);
  assert(status == napi_ok);

  JS_STRING_TO_C_CHAR(env, argv[2], op, status);

  bool out = jose_jwk_prm(NULL, (json_t *)json, req, op);

  napi_value result;

  status = napi_get_boolean(env, out, &result);
  assert(status == napi_ok);
  return result;
}
