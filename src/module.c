#include <node_api.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "jose/jose.h"
#include "lib/hooks.h"
/* #include "lib/jose_init.h" */
/* #include "lib/openssl/openssl_init.h" */
#include "lib/misc.h"
#include "lib/openssl/misc.h"
#include "module.h"

void jose_init(void) {
  openssl_misc_init();
  openssl_aescbch_init();
  openssl_aesgcm_init();
  openssl_aesgcmkw_init();
  openssl_aeskw_init();
  openssl_dir_init();
  openssl_ec_init();
  openssl_ecdh_init();
  openssl_ecdhes_init();
  openssl_ecdsa_init();
  openssl_hmac_init();
  openssl_pbes2_init();
  openssl_ecmr_init();
  openssl_hash_init();
  openssl_oct_init();
  openssl_rsa_init();
  openssl_rsaes_init();
  zlib_deflate_init();
  openssl_rsassa_init();
  jose_misc_init();
  jose_jwk_init();
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  locking_setup();
#endif /* OpenSSL < 1.1.0 */
}

void c_jose_json_decref(napi_env env, void* finalize_data, void* finalize_hint) {
  /* json_decrefp((json_t *) finalize_data); */
  json_decref((json_t *) finalize_data);
}

napi_value c_jose_json_loads(napi_env env, napi_callback_info info) {
  napi_value argv[1];
  size_t argc = 1;
  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  napi_status status;

  JS_STRING_TO_C_CHAR(env, argv[0], json, status);

  assert(status == napi_ok);

  json_t *c_json = json_loads(json, 0, NULL);

  napi_value result;

  napi_create_external(env, c_json, c_jose_json_decref, NULL, &result);

  return result;
}

napi_value c_jose_json_dumps(napi_env env, napi_callback_info info) {
  napi_value argv[1];
  size_t argc = 1;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

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

napi_value c_jose_jwk_thp_buf(napi_env env, napi_callback_info info) {
  napi_value argv[3];
  size_t argc = 3;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 3) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  napi_status status;

  napi_valuetype resultType;

  size_t jwkLength = 0;
  char* jwk = NULL;

  status = napi_typeof(env, argv[0], &resultType);
  assert(status == napi_ok);

  json_t *jwk_json = NULL;

  if(resultType != napi_null) {
    status = napi_get_value_string_utf8(env, argv[0], NULL, 0, &jwkLength); \
    assert(status == napi_ok);
    jwk = malloc(++jwkLength);
    status = napi_get_value_string_utf8(env, argv[0], jwk, jwkLength, &jwkLength); \
    assert(status == napi_ok);
    jwk_json = json_loads(jwk, 0, NULL);
    free(jwk);
    assert(jwk_json);
  }

  JS_STRING_TO_C_CHAR(env, argv[1], alg, status);
  assert(algLength);


  for (const jose_hook_alg_t *a = jose_hook_alg_list(); a; a = a->next) {
    /* if (a->kind == JOSE_HOOK_ALG_KIND_HASH) */
      fprintf(stdout, "%s\n", a->name);
  }


  if(algLength) {
    assert(jose_hook_alg_find(JOSE_HOOK_ALG_KIND_HASH, alg));
  }

  void *thp = NULL;
  size_t thp_size = 0;

  status = napi_typeof(env, argv[2], &resultType);
  assert(status == napi_ok);

  if(resultType != napi_null) {
    status = napi_get_arraybuffer_info(env, argv[2], &thp, &thp_size);
    assert(status == napi_ok);
  }

  size_t dlen = jose_jwk_thp_buf(NULL, jwk_json, alg, (uint8_t *) thp, thp_size);

  napi_value result;

  if(!thp) {
    status = napi_create_uint32(env, dlen, &result);
    assert(status == napi_ok);
    return result;
  }

  return argv[2];
}

napi_value Init(napi_env env, napi_value exports) {
  jose_init();

  // Module initialization code goes here
  napi_status status;

  napi_property_descriptor descArray[] = {
    DECLARE_NAPI_METHOD("jose_jwk_thp_buf", c_jose_jwk_thp_buf),
    DECLARE_NAPI_METHOD("jose_json_loads", c_jose_json_loads),
    DECLARE_NAPI_METHOD("jose_json_dumps", c_jose_json_dumps),
  };

  size_t n_desc = sizeof(descArray) / sizeof(napi_property_descriptor);

  status = napi_define_properties(env, exports, n_desc, descArray);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
