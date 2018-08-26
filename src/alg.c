#include <node_api.h>
#include <string.h>
#include <jose/jose.h>
#include <assert.h>
#include <lib/hooks.h>

#include "module.h"


NAPI_METHOD(c_jose_alg_foreach) {
  napi_value thisVal;

  NAPI_METHOD_THIS_ARG(1, thisVal);

  napi_status status;
  napi_handle_scope scope;

  for (const jose_hook_alg_t *a = jose_hook_alg_list(); a; a = a->next) {
    status = napi_open_handle_scope(env, &scope);
    assert(status == napi_ok);

    napi_value algKind;
    napi_value algName;

    status = napi_create_uint32(env, a->kind, &algKind);
    assert(status == napi_ok);

    status = napi_create_string_utf8(env, a->name, NAPI_AUTO_LENGTH, &algName);
    assert(status == napi_ok);

    napi_value cbArgV[] = {algKind, algName};

    status = napi_call_function(env, thisVal, argv[0], 2, cbArgV, NULL);
    assert(status == napi_ok);

    status = napi_close_handle_scope(env, scope);
    assert(status == napi_ok);
  }

  napi_value result;
  napi_get_undefined(env, &result);
  return result;
}


// create json type enumeration: json_type.JSON_ARRAY, json_type.JSON_STRING, ...
napi_value alg_kind_init(napi_env env) {
  napi_value result;

  napi_status status = napi_create_object(env, &result);
  assert(status == napi_ok);

  create_uint32_value(JOSE_HOOK_ALG_KIND_HASH);
  create_uint32_value(JOSE_HOOK_ALG_KIND_SIGN);
  create_uint32_value(JOSE_HOOK_ALG_KIND_WRAP);
  create_uint32_value(JOSE_HOOK_ALG_KIND_ENCR);
  create_uint32_value(JOSE_HOOK_ALG_KIND_COMP);
  create_uint32_value(JOSE_HOOK_ALG_KIND_EXCH);

  const napi_property_descriptor desc[] = {
    DECLARE_NAPI_CONSTANT("JOSE_HOOK_ALG_KIND_HASH", _JOSE_HOOK_ALG_KIND_HASH),
    DECLARE_NAPI_CONSTANT("JOSE_HOOK_ALG_KIND_SIGN", _JOSE_HOOK_ALG_KIND_SIGN),
    DECLARE_NAPI_CONSTANT("JOSE_HOOK_ALG_KIND_WRAP", _JOSE_HOOK_ALG_KIND_WRAP),
    DECLARE_NAPI_CONSTANT("JOSE_HOOK_ALG_KIND_ENCR", _JOSE_HOOK_ALG_KIND_ENCR),
    DECLARE_NAPI_CONSTANT("JOSE_HOOK_ALG_KIND_COMP", _JOSE_HOOK_ALG_KIND_COMP),
    DECLARE_NAPI_CONSTANT("JOSE_HOOK_ALG_KIND_EXCH", _JOSE_HOOK_ALG_KIND_EXCH),
  };

  size_t n_desc = sizeof(desc) / sizeof(napi_property_descriptor);

  status = napi_define_properties(env, result, n_desc, desc);

  return result;
}

