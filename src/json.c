#include <node_api.h>
#include <string.h>
#include <jose/jose.h>
#include <assert.h>

#include "module.h"


void c_jose_json_decref(napi_env env, void* finalize_data, void* finalize_hint) {
  /* json_decrefp((json_t *) finalize_data); */
  json_decref((json_t *) finalize_data);
}

NAPI_METHOD(c_jose_json_loads) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  JS_STRING_TO_C_CHAR(env, argv[0], json, status);

  json_t *c_json = json_loads(json, 0, NULL);

  napi_value result;

  napi_create_external(env, c_json, c_jose_json_decref, NULL, &result);

  return result;
}

NAPI_METHOD(c_jose_json_dumps) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *c_json;
  status = napi_get_value_external(env, argv[0], &c_json);

  assert(status == napi_ok);

  char* char_json = json_dumps((json_t * )c_json, 0);

  napi_value result;
  napi_create_string_utf8(env, char_json, strlen(char_json), &result);

  free(char_json);

  return result;
}
