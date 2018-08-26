#pragma once

// MACROS
#define DECLARE_NAPI_METHOD(name, func)         \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

#define DECLARE_NAPI_CONSTANT(name, value)      \
  { name, 0, 0, 0, 0, value, napi_default, 0 }

#define JS_STRING_TO_C_CHAR(ENV, STR, NAME, STATUS)                    \
  size_t NAME##Length;                                                  \
  STATUS = napi_get_value_string_utf8(ENV, STR, NULL, 0, &NAME##Length); \
  assert(STATUS == napi_ok);                                            \
  NAME##Length += 1;                                                    \
  char NAME[NAME##Length];                                              \
  STATUS = napi_get_value_string_utf8(ENV, STR, NAME, NAME##Length, &NAME##Length); \
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

// helper macro
#define create_uint32_value(name)               \
  napi_value _##name;                           \
  napi_create_uint32(env, name, &_##name);


// ===
// ALG

napi_value alg_kind_init(napi_env env);
NAPI_METHOD(c_jose_alg_foreach);

// =====
// BASE64
NAPI_METHOD(c_jose_b64_enc_sbuf);
NAPI_METHOD(c_jose_b64_enc_bbuf);
NAPI_METHOD(c_jose_b64_dec_buf);

// =====
// JSON
napi_value json_type_init(napi_env env);
napi_value json_encoding_init(napi_env env);
void c_jose_json_decref(napi_env env, void* finalize_data, void* finalize_hint);
void c_jose_io_decref(napi_env env, void* finalize_data, void* finalize_hint);

NAPI_METHOD(c_jose_json_loads);
NAPI_METHOD(c_jose_json_dumps);
NAPI_METHOD(c_jose_json_foreach);
NAPI_METHOD(c_jose_json_typeof);
NAPI_METHOD(c_jose_json_get);
NAPI_METHOD(c_jose_json_array_get);
NAPI_METHOD(c_jose_json_value_get);
NAPI_METHOD(c_jose_json_number_value);
NAPI_METHOD(c_jose_json_is_number);
NAPI_METHOD(c_jose_json_object_update);
NAPI_METHOD(c_jose_json_object_set_new);
NAPI_METHOD(c_jose_json_construct);


// =====
// JWK
NAPI_METHOD(c_jose_jwk_thp_buf);
NAPI_METHOD(c_jose_jwk_gen);
NAPI_METHOD(c_jose_jwk_exc);
NAPI_METHOD(c_jose_jwk_pub);
NAPI_METHOD(c_jose_jwk_prm);

// =====
// JWE

NAPI_METHOD(c_jose_jwe_hdr);
NAPI_METHOD(c_jose_jwe_enc);
NAPI_METHOD(c_jose_jwe_dec);
NAPI_METHOD(c_jose_jwe_enc_jwk);
NAPI_METHOD(c_jose_jwe_dec_jwk);
NAPI_METHOD(c_jose_jwe_enc_io);
NAPI_METHOD(c_jose_jwe_dec_io);
NAPI_METHOD(c_jose_jwe_enc_cek);
NAPI_METHOD(c_jose_jwe_dec_cek);
NAPI_METHOD(c_jose_jws_hdr);
NAPI_METHOD(c_jose_jws_sig);
NAPI_METHOD(c_jose_jws_sig_io);
NAPI_METHOD(c_jose_jws_ver);
NAPI_METHOD(c_jose_jws_ver_io);
