#pragma once

#ifndef MODULE_H
#define MODULE_H

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



#endif /* MODULE_H */
