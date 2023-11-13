#include "types.h"
#include "complex.h"

#ifndef TI_UTIL_H
#define TI_UTIL_H

u16 parsei(const char* input);
f64 parsef(const char* input);
complex parse_complex(const char* input);
const char* ftos(f64 value);

#endif
