#pragma once

// MACROS
#define DECLARE_NAPI_METHOD(name, func)         \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

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


// =====
// JSON
void c_jose_json_decref(napi_env env, void* finalize_data, void* finalize_hint);

NAPI_METHOD(c_jose_json_loads);
NAPI_METHOD(c_jose_json_dumps);


// =====
// JWK
NAPI_METHOD(c_jose_jwk_thp_buf);


// =====
// BASE64
NAPI_METHOD(c_jose_b64_enc_buf);
NAPI_METHOD(c_jose_b64_dec_buf);
