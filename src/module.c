#include <node_api.h>
#include <assert.h>

#include "jose/jose.h"
#include "module.h"

#define DECLARE_NAPI_METHOD(name, func)         \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value c_jose_jwk_prm(napi_env env, napi_callback_info info) {
  napi_value argv[3];
  size_t argc = 3;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 3) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  napi_status status;

  size_t strBufSize;
  status = napi_get_value_string_utf8(env, argv[0], NULL, 0, &strBufSize);
  assert(status == napi_ok);
  char json_data[strBufSize];
  size_t strSize;
  status = napi_get_value_string_utf8(env, argv[0], json_data, strBufSize, &strSize);
  assert(status == napi_ok);

  napi_value world;
  status = napi_create_string_utf8(env, "world", 5, &world);
  assert(status == napi_ok);
  return world;
}

napi_value Init(napi_env env, napi_value exports) {
  // Module initialization code goes here
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("jose_jwk_prm", c_jose_jwk_prm);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
