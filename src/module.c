#include "jose/jose.h"
#include <node_api.h>

napi_value Init(napi_env env, napi_value exports) {
  // Module initialization code goes here
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
