#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <jose/jose.h>
#include <lib/hooks.h>

#include "module.h"

NAPI_METHOD(c_jose_io_buffer) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  size_t buf_size = 0;
  void* buf = NULL;

  status = napi_get_buffer_info(env, argv[0], &buf, &buf_size);
  assert(status == napi_ok);

  jose_io_t *out = jose_io_buffer(NULL, &buf,(size_t *) &buf_size);

  napi_value result;

  if(!out) {
    status = napi_get_undefined(env, &result);
    assert(status == napi_ok);
    return result;
  }

  status = napi_create_external(env, out, c_jose_io_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_io_malloc) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  size_t buf_size = 0;
  void* buf = NULL;

  status = napi_get_value_uint32(env, argv[0], (uint32_t *) &buf_size);
  assert(status == napi_ok);

  jose_io_t *out = jose_io_malloc(NULL, &buf, &buf_size);

  napi_value result;

  if(!out) {
    status = napi_get_undefined(env, &result);
    assert(status == napi_ok);
    return result;
  }

  status = napi_create_external(env, out, c_jose_io_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}

