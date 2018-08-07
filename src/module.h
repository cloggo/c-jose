#pragma once

// MACROS
#define DECLARE_NAPI_METHOD(name, func)         \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

#define DECLARE_NAPI_CONSTANT(name, value)      \
  { name, 0, 0, 0, 0, value, napi_default, 0 }

#define JS_STRING_TO_C_CHAR(ENV, JSON, NAME, STATUS)                    \
  size_t NAME##Length;                                                  \
  STATUS = napi_get_value_string_utf8(ENV, JSON, NULL, 0, &NAME##Length); \
  assert(STATUS == napi_ok);                                            \
  NAME##Length += 1;                                                    \
  char NAME[NAME##Length];                                              \
  STATUS = napi_get_value_string_utf8(ENV, JSON, NAME, NAME##Length, &NAME##Length); \
  assert(STATUS == napi_ok);

#define NAPI_METHOD(name)                                 \
  napi_value name(napi_env env, napi_callback_info info)

#define NAPI_METHOD_ARG(MAX_ARGC)                         \
  napi_value argv[MAX_ARGC];                              \
  size_t argc = MAX_ARGC;                                 \
                                                          \
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);   \
                                                          \
  if (argc < MAX_ARGC) {                                  \
    napi_throw_error(env, "EINVAL", "Too few arguments"); \
    return NULL;                                          \
  }

#define NAPI_METHOD_THIS_ARG(MAX_ARGC, THIS_ARG)               \
  napi_value argv[MAX_ARGC];                              \
  size_t argc = MAX_ARGC;                                 \
                                                          \
  napi_get_cb_info(env, info, &argc, argv, &THIS_ARG, NULL); \
                                                          \
  if (argc < MAX_ARGC) {                                  \
    napi_throw_error(env, "EINVAL", "Too few arguments"); \
    return NULL;                                          \
  }


#define __AUTO_CLEANUP__(cleanup_func) \
  __attribute__((cleanup(cleanup_func)))

bool free_char(char **p);
bool free_json(json_t **p);


// =====
// JSON
napi_value json_type_init(napi_env env);
void c_jose_json_decref(napi_env env, void* finalize_data, void* finalize_hint);

NAPI_METHOD(c_jose_json_loads);
NAPI_METHOD(c_jose_json_dumps);
NAPI_METHOD(c_jose_json_foreach);
NAPI_METHOD(c_jose_json_typeof);


// =====
// JWK
NAPI_METHOD(c_jose_jwk_thp_buf);


// =====
// BASE64
NAPI_METHOD(c_jose_b64_enc_sbuf);
NAPI_METHOD(c_jose_b64_enc_bbuf);
NAPI_METHOD(c_jose_b64_dec_buf);
