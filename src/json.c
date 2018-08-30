#include <node_api.h>
#include <string.h>
#include <jose/jose.h>
#include <assert.h>

#include "module.h"


void c_jose_json_decref(napi_env env, void* finalize_data, void* finalize_hint) {
  /* json_decrefp((json_t *) finalize_data); */
  json_decref((json_t *) finalize_data);
}


// string -> json
NAPI_METHOD(c_jose_json_loads) {
  napi_value argv[2];
  size_t argc = 2;
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  napi_status status;

  uint32_t flags = 0;

  switch(argc) {
  case 1:
    break;
  case 2:
    status = napi_get_value_uint32(env, argv[1], &flags);
    assert(status == napi_ok);
    break;
  default:
    napi_throw_error(env, "EINVAL", "Invalid arguments"); \
    return NULL;                                          \
  }

  JS_STRING_TO_C_CHAR(env, argv[0], json, status);

  json_t *c_json = json_loads(json, flags, NULL);

  napi_value result;

  napi_create_external(env, c_json, c_jose_json_decref, NULL, &result);

  return result;
}


// json -> utf8 string
NAPI_METHOD(c_jose_json_dumps) {
  napi_value argv[2];
  size_t argc = 2;
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {                                  \
    napi_throw_error(env, "EINVAL", "Required at least one argument - JSON"); \
    return NULL;                                          \
  }

  napi_status status;

  uint32_t flags = 0;
  void *c_json;

  status = napi_get_value_external(env, argv[0], &c_json);
  assert(status == napi_ok);

  if (argc == 2) {
    status = napi_get_value_uint32(env, argv[1], &flags);
    assert(status == napi_ok);
  }

  char* char_json = json_dumps((json_t * )c_json, flags);

  napi_value result;
  napi_create_string_utf8(env, char_json, strlen(char_json), &result);

  free(char_json);

  return result;
}


// foreach(json_array, callback) => foreach index, value: call callback(index, value)
// return: failed false if json_aray is array
//         failed true if not array

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


// create json type enumeration: json_type.JSON_ARRAY, json_type.JSON_STRING, ...
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


// typeof(json) -> json type
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

// typeof(json) -> json type
NAPI_METHOD(c_jose_json_deep_copy) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *json_in;
  status = napi_get_value_external(env, argv[0], &json_in);
  assert(status == napi_ok);

  json_t * json_out = json_deep_copy(json_in);

  napi_value result;

  if(!json_out) {
    napi_get_undefined(env, &result);
    return result;
  }

  status = napi_create_external(env, json_out, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}

NAPI_METHOD(c_jose_json_object) {
  /* NAPI_METHOD_ARG(0); */
  napi_status status;

  json_t* json = json_object();

  napi_value result;

  if(!json) {
    napi_get_undefined(env, &result);
    return result;
  }

  status = napi_create_external(env, json, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_array) {
  /* NAPI_METHOD_ARG(0); */
  napi_status status;

  json_t* json = json_array();

  napi_value result;

  if(!json) {
    napi_get_undefined(env, &result);
    return result;
  }

  status = napi_create_external(env, json, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}

// array_get(json_array, index) -> return json at index
NAPI_METHOD(c_jose_json_array_get) {
  NAPI_METHOD_ARG(2);

  napi_status status;

  void *root;
  status = napi_get_value_external(env, argv[0], &root);
  assert(status == napi_ok);

  uint32_t index;
  status = napi_get_value_uint32(env, argv[1], &index);
  assert(status == napi_ok);

  json_t* child = json_array_get((json_t *)root, index);

  napi_value result;

  if(!child) {
    napi_get_undefined(env, &result);
    return result;
  }

  status = napi_create_external(env, child, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}


// get(json, key)
// return: value of the corresponding key
//         null if key doesn't exists

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
    napi_get_undefined(env, &result);
    return result;
  }

  status = napi_create_external(env, child, c_jose_json_decref, NULL, &result);
  assert(status == napi_ok);

  return result;
}


// assumed it is a value type
// the actual value of data
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
    status = napi_get_undefined(env, &result);
    assert(status == napi_ok);
    return result;
  }

  return result;
}


// is_value(json) -> true: not (type object or type array)
NAPI_METHOD(c_jose_json_is_value) {
  NAPI_METHOD_ARG(1);

  napi_status status;

  void *root;
  status = napi_get_value_external(env, argv[0], &root);
  assert(status == napi_ok);

  napi_value result;

  status = napi_get_boolean(env,
                            !(json_is_object((json_t *) root)
                              || json_is_array((json_t *) root)),
                            &result);
  assert(status == napi_ok);

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

NAPI_METHOD(c_jose_json_object_update) {

  NAPI_METHOD_ARG(2);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  void *other;
  status = napi_get_value_external(env, argv[1], &other);
  assert(status == napi_ok);


  int value = json_object_update((json_t *)json, (json_t *)other);

  napi_value result;

  status = napi_create_int32(env, value, &result);
  assert(status == napi_ok);

  return result;
}

NAPI_METHOD(c_jose_json_array_append_new) {

  NAPI_METHOD_ARG(2);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  void *value;
  status = napi_get_value_external(env, argv[1], &value);
  assert(status == napi_ok);


  int out = json_array_append_new((json_t *)json, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}

NAPI_METHOD(c_jose_json_array_extend) {

  NAPI_METHOD_ARG(2);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  void *value;
  status = napi_get_value_external(env, argv[1], &value);
  assert(status == napi_ok);


  int out = json_array_extend((json_t *)json, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}



NAPI_METHOD(c_jose_json_array_append) {

  NAPI_METHOD_ARG(2);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  void *value;
  status = napi_get_value_external(env, argv[1], &value);
  assert(status == napi_ok);


  int out = json_array_append((json_t *)json, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_array_remove) {

  NAPI_METHOD_ARG(2);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  uint32_t index;
  status = napi_get_value_uint32(env, argv[1], &index);
  assert(status == napi_ok);

  int out = json_array_remove((json_t *)json, (size_t) index);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_object_del) {

  NAPI_METHOD_ARG(2);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  JS_STRING_TO_C_CHAR(env, argv[1], key, status);

  int out = json_object_del((json_t *)json, key);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_array_clear) {

  NAPI_METHOD_ARG(1);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  int out = json_array_clear((json_t *)json);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_object_clear) {

  NAPI_METHOD_ARG(1);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  int out = json_object_clear((json_t *)json);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_array_insert_new) {

  NAPI_METHOD_ARG(3);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  uint32_t index = -1;
  status = napi_get_value_uint32(env, argv[1], &index);
  assert(status == napi_ok);

  void *value;
  status = napi_get_value_external(env, argv[2], &value);
  assert(status == napi_ok);


  int out = json_array_insert_new((json_t *)json, index, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_array_insert) {

  NAPI_METHOD_ARG(3);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  uint32_t index = -1;
  status = napi_get_value_uint32(env, argv[1], &index);
  assert(status == napi_ok);

  void *value;
  status = napi_get_value_external(env, argv[2], &value);
  assert(status == napi_ok);


  int out = json_array_insert((json_t *)json, index, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_object_set) {

  NAPI_METHOD_ARG(3);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  JS_STRING_TO_C_CHAR(env, argv[1], key, status);

  void *value;
  status = napi_get_value_external(env, argv[2], &value);
  assert(status == napi_ok);


  int out = json_object_set((json_t *)json, key, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}

NAPI_METHOD(c_jose_json_object_set_new) {

  NAPI_METHOD_ARG(3);

  napi_status status;

  void *json;
  status = napi_get_value_external(env, argv[0], &json);
  assert(status == napi_ok);

  JS_STRING_TO_C_CHAR(env, argv[1], key, status);

  void *value;
  status = napi_get_value_external(env, argv[2], &value);
  assert(status == napi_ok);


  int out = json_object_set_new((json_t *)json, key, (json_t *)value);

  napi_value result;

  status = napi_create_int32(env, out, &result);
  assert(status == napi_ok);

  return result;
}


NAPI_METHOD(c_jose_json_create) {
  napi_value argv[2];
  size_t argc = 2;
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {                                  \
    napi_throw_error(env, "EINVAL", "Required at least one argument - JSON type"); \
    return NULL;                                          \
  }

  napi_status status;

  uint32_t t = 0;
  status = napi_get_value_uint32(env, argv[0], &t);
  assert(status == napi_ok);
  assert(t <= JSON_NULL);

  json_t *json = NULL;

  union {
    char* s;
    double r;
    int64_t i;
  } value;

  napi_value result;

  switch(t) {
  case JSON_STRING:
    assert(argc == 2);
    size_t valueLength = 0;
    status = napi_get_value_string_utf8(env, argv[1], NULL, 0, &valueLength); \
    assert(status == napi_ok);
    value.s = malloc(++valueLength);
    status = napi_get_value_string_utf8(env, argv[1], value.s, valueLength, &valueLength); \
    assert(status == napi_ok);
    json = json_string(value.s);
    free(value.s);
    break;
  case JSON_INTEGER:
    assert(argc == 2);
    status = napi_get_value_int64(env, argv[1], &value.i);
    assert(status == napi_ok);
    json = json_integer(value.i);
    break;
  case JSON_REAL:
    assert(argc == 2);
    status = napi_get_value_double(env, argv[1], &value.r);
    assert(status == napi_ok);
    json = json_real(value.r);
    break;
  case JSON_TRUE:
    json = json_true();
    break;
  case JSON_FALSE:
    json = json_false();
    break;
  case JSON_OBJECT:
    json = json_object();
    break;
  case JSON_ARRAY:
    json = json_array();
    break;
  default:
    status = napi_get_undefined(env, &result);
    assert(status == napi_ok);
    return result;
  }

  status = napi_create_external(env, json, c_jose_json_decref, NULL, &result);

  return result;
}

napi_value json_encoding_init(napi_env env) {
  napi_value result;

  napi_status status = napi_create_object(env, &result);
  assert(status == napi_ok);

  create_uint32_value(JSON_MAX_INDENT);
  create_uint32_value(JSON_COMPACT);
  create_uint32_value(JSON_ENSURE_ASCII);
  create_uint32_value(JSON_SORT_KEYS);
  create_uint32_value(JSON_PRESERVE_ORDER);
  create_uint32_value(JSON_ENCODE_ANY);
  create_uint32_value(JSON_ESCAPE_SLASH);
  create_uint32_value(JSON_EMBED);

  const napi_property_descriptor desc[] = {
    DECLARE_NAPI_CONSTANT("JSON_MAX_INDENT", _JSON_MAX_INDENT),
    DECLARE_NAPI_CONSTANT("JSON_COMPACT",_JSON_COMPACT),
    DECLARE_NAPI_CONSTANT("JSON_ENSURE_ASCII",_JSON_ENSURE_ASCII),
    DECLARE_NAPI_CONSTANT("JSON_SORT_KEYS",_JSON_SORT_KEYS),
    DECLARE_NAPI_CONSTANT("JSON_PRESERVE_ORDER",_JSON_PRESERVE_ORDER),
    DECLARE_NAPI_CONSTANT("JSON_ENCODE_ANY",_JSON_ENCODE_ANY),
    DECLARE_NAPI_CONSTANT("JSON_ESCAPE_SLASH",_JSON_ESCAPE_SLASH),
    DECLARE_NAPI_CONSTANT("JSON_EMBED",_JSON_EMBED),
  };

  size_t n_desc = sizeof(desc) / sizeof(napi_property_descriptor);

  status = napi_define_properties(env, result, n_desc, desc);
  assert(status == napi_ok);

  return result;
}

