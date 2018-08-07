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

NAPI_METHOD(c_jose_json_foreach) {
  napi_value thisVal;

  NAPI_METHOD_THIS_ARG(2, thisVal);

  napi_status status;

  void *c_json;
  status = napi_get_value_external(env, argv[0], &c_json);

  napi_value result;

  bool is_array = json_is_array((json_t *)c_json);
  napi_get_boolean(env, !is_array, &result);

  if(!is_array){
    return result;
  }

  assert(status == napi_ok);

  size_t index;
  json_t *value;

  json_array_foreach((json_t*) c_json, index, value) {
    napi_handle_scope scope;
    napi_status status = napi_open_handle_scope(env, &scope);
    assert(status == napi_ok);

    napi_value jValue;
    napi_value jIndex;

    napi_create_uint32(env, index, &jIndex);
    napi_create_external(env, value, c_jose_json_decref, NULL, &jValue);

    napi_value jArgv[] = {jIndex, jValue};

    status = napi_call_function(env, thisVal, argv[1], 2, jArgv, NULL);

    status = napi_close_handle_scope(env, scope);
    assert(status == napi_ok);
  }

  return result;
}

#define create_uint32_value(name)               \
  napi_value _##name;                             \
  napi_create_uint32(env, name, &_##name);

napi_value json_type_init(napi_env env) {
  napi_value result;

  napi_status status = napi_create_object(env, &result);
  assert(status == napi_ok);

  create_uint32_value(JSON_STRING);
  create_uint32_value(JSON_INTEGER);
  create_uint32_value(JSON_ARRAY);
  create_uint32_value(JSON_REAL);
  create_uint32_value(JSON_TRUE);
  create_uint32_value(JSON_FALSE);
  create_uint32_value(JSON_NULL);
  create_uint32_value(JSON_OBJECT);

  const napi_property_descriptor desc[] = {
    DECLARE_NAPI_CONSTANT("JSON_STRING", _JSON_STRING),
    DECLARE_NAPI_CONSTANT("JSON_INTEGER", _JSON_INTEGER),
    DECLARE_NAPI_CONSTANT("JSON_ARRAY", _JSON_ARRAY),
    DECLARE_NAPI_CONSTANT("JSON_REAL", _JSON_REAL),
    DECLARE_NAPI_CONSTANT("JSON_TRUE", _JSON_TRUE),
    DECLARE_NAPI_CONSTANT("JSON_FALSE", _JSON_FALSE),
    DECLARE_NAPI_CONSTANT("JSON_NULL", _JSON_NULL),
    DECLARE_NAPI_CONSTANT("JSON_OBJECT", _JSON_OBJECT),
  };

  size_t n_desc = sizeof(desc) / sizeof(napi_property_descriptor);

  status = napi_define_properties(env, result, n_desc, desc);

  return result;
}

NAPI_METHOD(c_jose_json_typeof) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *c_json;
  status = napi_get_value_external(env, argv[0], &c_json);
  assert(status == napi_ok);

  napi_value result;
  status = napi_create_uint32(env, json_typeof((json_t *)c_json), &result);
  assert(status == napi_ok);
  return result;
}

NAPI_METHOD(c_jose_json_get) {
  NAPI_METHOD_ARG(2);

  napi_status status;

  void *root;
  status = napi_get_value_external(env, argv[0], &root);
  assert(status == napi_ok);

  JS_STRING_TO_C_CHAR(env, argv[1], key, status);

  json_t* child = json_object_get((json_t *)root, key);

  napi_value result;

  if(!child) {
    napi_get_null(env, &result);
    return result;
  }

  status = napi_create_external(env, child, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}

NAPI_METHOD(c_jose_json_value_get) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *root;
  status = napi_get_value_external(env, argv[0], &root);
  assert(status == napi_ok);

  json_type t = json_typeof((json_t *)root);

  napi_value result;

  switch(t) {
  case JSON_STRING:
    status = napi_create_string_utf8(env,
                                     json_string_value((json_t *)root),
                                     json_string_length((json_t *)root),
                                     &result);
    assert(status == napi_ok);
    break;
  case JSON_INTEGER:
    status = napi_create_int64(env, json_integer_value((json_t *)root), &result);
    assert(status == napi_ok);
    break;
  case JSON_REAL:
    status = napi_create_double(env, json_real_value((json_t *)root), &result);
    assert(status == napi_ok);
    break;
  case JSON_TRUE:
    status = napi_get_boolean(env, json_boolean_value((json_t *)root), &result);
    assert(status == napi_ok);
    break;
  case JSON_FALSE:
    status = napi_get_boolean(env, json_boolean_value((json_t *)root), &result);
    assert(status == napi_ok);
    break;
  default:
    status = napi_get_null(env, &result);
    assert(status == napi_ok);
  }

  return result;
}

NAPI_METHOD(c_jose_json_is_number) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *root;
  status = napi_get_value_external(env, argv[0], &root);
  assert(status == napi_ok);

  napi_value result;

  status = napi_get_boolean(env, json_is_number((json_t *) root), &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_number_value) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *root;
  status = napi_get_value_external(env, argv[0], &root);
  assert(status == napi_ok);

  napi_value result;

  status = napi_create_double(env, json_number_value((json_t *)root), &result);
  assert(status == napi_ok);

  return result;
}
