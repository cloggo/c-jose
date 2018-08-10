#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <jose/jose.h>
#include <lib/hooks.h>

#include "module.h"

NAPI_METHOD(c_jose_jws_hdr) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void* sig = NULL;

  status = napi_get_value_external(env, argv[0], &sig);
  assert(status == napi_ok);

  json_t *out = jose_jws_hdr((json_t*)sig);

  napi_value result;

  status = napi_create_external(env, out, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_jws_sig) {
  NAPI_METHOD_ARG(3);

  napi_status status;

  napi_valuetype resulttype;

  void* jws = NULL;
  void* sig = NULL;
  void* jwk = NULL;

  status = napi_get_value_external(env, argv[0], &jws);
  assert(status == napi_ok);

  status = napi_typeof(env, argv[1], &resulttype);
  assert(status == napi_ok);

  if(resulttype != napi_null) {
    status = napi_get_value_external(env, argv[1], &sig);
    assert(status == napi_ok);
  }

  status = napi_get_value_external(env, argv[2], &jwk);
  assert(status == napi_ok);

  bool out = jose_jws_sig(NULL, (json_t*)jws, (json_t*)sig, (json_t *)jwk);

  napi_value result;

  status = napi_get_boolean(env, out, &result);
  assert(status == napi_ok);
  return result;
}


NAPI_METHOD(c_jose_jws_sig_io) {
  NAPI_METHOD_ARG(3);

  napi_status status;

  napi_valuetype resulttype;

  void* jws = NULL;
  void* sig = NULL;
  void* jwk = NULL;

  status = napi_get_value_external(env, argv[0], &jws);
  assert(status == napi_ok);

  status = napi_typeof(env, argv[1], &resulttype);
  assert(status == napi_ok);

  if(resulttype != napi_null) {
    status = napi_get_value_external(env, argv[1], &sig);
    assert(status == napi_ok);
  }

  status = napi_get_value_external(env, argv[2], &jwk);
  assert(status == napi_ok);

  jose_io_t* out = jose_jws_sig_io(NULL, (json_t*)jws, (json_t*)sig, (json_t *)jwk);

  napi_value result;

  if(!out) {
    status = napi_get_null(env, &result);
    assert(status == napi_ok);
    return result;
  }

  status = napi_create_external(env, out, c_jose_io_decref, NULL, &result);
  assert(status == napi_ok);

  return result;

}
