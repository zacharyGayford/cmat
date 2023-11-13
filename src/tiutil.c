#include "tiutil.h"

#include <string.h>
#include <ti/real.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <ti/tokens.h>

#include "types.h"

#define DIGITS 5
#define INPUT_SIZE 20
#define SIMPLE 12345
#define IMAGINARY 12347
#define REAL 12348

complex parse_complex(const char* input) {

	static char input_buf[INPUT_SIZE + 1];
	memset(input_buf, 0, INPUT_SIZE);
	complex result;
	u16 divider = SIMPLE;
	u16 len = strlen(input);

	char* plus_sign = strchr(input, OS_TOK_ADD);
	if (plus_sign)
		divider = strchr(input, OS_TOK_ADD) - input;

	memcpy(input_buf, input, len);
	if (divider == SIMPLE) {
		if (input_buf[len - 1 ] == OS_TOK_IMAGINARY) {
			divider = IMAGINARY;
			input_buf[len - 1] = 0; // remove imaginary number
		} else {
			divider = REAL;
		}
	} else {
		input_buf[len - 1] = 0;
	}

	switch(divider) {
		case IMAGINARY:
			result.real = 0;
			result.imag = parsef(input_buf);
			break;
		case REAL:
			result.real = parsef(input_buf);
			result.imag = 0;
			break;
		default:
			input_buf[divider] = 0; // split string
			result.real = parsef(input_buf);
			result.imag = parsef(input_buf + divider + 1);
			break;
	}

	return result;

}

u16 parsei(const char* input) {

	size_t len = strlen(input);
	u32 result = 0;

	for (u16 i = 0; i < len; i ++) {
		result *= 10;
		result += (input[i] - '0');			
	}

	return result;

}

f64 parsef(const char* input) {

	size_t len = strlen(input);
	i32 integer = 0;
	u32 mantissa = 0;
	u16 start = 0;

	if (input[0] == OS_TOK_SUBTRACT)
		start = 1;

	while (input[len - mantissa - 1] != OS_TOK_DECIMAL_POINT) {
		mantissa ++;
		if (mantissa > len) {
			mantissa = 0;
			break;
		}
	}

	for (u16 i = start; i < len; i ++) {
		if (input[i] == OS_TOK_DECIMAL_POINT) continue;
		integer *= 10;
		integer += (input[i] - '0');			
	}

	f64 result = (f64)integer;
	while(mantissa--)
		result /= 10.0f;

	if (start)
		return -result;
	return result;

}

const char* ftos(f64 value) {
	
	static char result[DIGITS + 1];
	real_t tmp_real = os_FloatToReal(value);
	os_RealToStr(result, &tmp_real, 8, 1, 2);
	/*
	if (value < 0) result[0] = '-';
	else			result[0] = ' ';
	*/

	return result;

}

#undef DIGITS
#undef INPUT_SIZE
#undef SIMPLE
#undef IMAGINARY
#undef REAL
