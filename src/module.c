#include <node_api.h>
#include <assert.h>
#include <stdio.h>

#include <lib/misc.h>
#include <lib/openssl/misc.h>

#include "module.h"

// TODO:
// * callback function to loop array
// * encode base64
// * decode base64

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

bool free_char(char **p) {
  printf("freeing char memory\n");

  if(*p) {
    free(*p);
  }

  return 0;
}

bool free_json(json_t **p) {
  if(*p) {
    json_decref(*p);
  }

  return 0;
}

napi_value Init(napi_env env, napi_value exports) {
  jose_init();

  // Module initialization code goes here
  napi_status status;

  napi_property_descriptor descArray[] = {
    DECLARE_NAPI_CONSTANT("jose_json_type", json_type_init(env)),
    DECLARE_NAPI_METHOD("jose_jwk_thp_buf", c_jose_jwk_thp_buf),
    DECLARE_NAPI_METHOD("jose_json_loads", c_jose_json_loads),
    DECLARE_NAPI_METHOD("jose_json_dumps", c_jose_json_dumps),
    DECLARE_NAPI_METHOD("jose_b64_dec_buf", c_jose_b64_dec_buf),
    DECLARE_NAPI_METHOD("jose_b64_enc_sbuf", c_jose_b64_enc_sbuf),
    DECLARE_NAPI_METHOD("jose_b64_enc_bbuf", c_jose_b64_enc_bbuf),
    DECLARE_NAPI_METHOD("jose_json_foreach", c_jose_json_foreach),
    DECLARE_NAPI_METHOD("jose_json_typeof", c_jose_json_typeof),
    DECLARE_NAPI_METHOD("jose_json_get", c_jose_json_get),
    DECLARE_NAPI_METHOD("jose_json_array_get", c_jose_json_array_get),
    DECLARE_NAPI_METHOD("jose_json_value_get", c_jose_json_value_get),
    DECLARE_NAPI_METHOD("jose_json_is_number", c_jose_json_is_number),
    DECLARE_NAPI_METHOD("jose_json_number_value", c_jose_json_number_value),
    DECLARE_NAPI_METHOD("jose_jwk_gen", c_jose_jwk_gen),
  };

  size_t n_desc = sizeof(descArray) / sizeof(napi_property_descriptor);

  status = napi_define_properties(env, exports, n_desc, descArray);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
