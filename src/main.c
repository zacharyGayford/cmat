#include <stdlib.h>
#include <string.h>
#include <ti/real.h>
#include <ti/screen.h>
#include <ti/getcsc.h>
#include <ti/vars.h>
#include <ti/getkey.h>
#include <stdbool.h>

#include "types.h"
#include "matrix.h"
#include "complex.h"
#include "cmatrix.h"
#include "tiutil.h"

#define DIGITS 5
#define INPUT_SIZE 20
#define MATRIX_SIZE 10 
#define ONLY_REAL 12345
#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)
static char input[INPUT_SIZE + 1];

u16 wrap(u16 val, u16 min, u16 max) {
	if (val < min) return max;
	if (val > max) return min;
	return val;
}

void print_prompt(u16 row, u16 col) {
	char prompt[6] = "(T,T)\0";
	prompt[1] = row + '0';
	prompt[3] = col + '0';
	os_PutStrFull(prompt);
}

void cmat_input(cmat* m, const char* prompt) {

	os_FontSelect(os_LargeFont);

	u16 key;
	bool wrong_key = false;
	bool isInput = true;
	u16 row = 0, col = 0;

	while (isInput) {
		if (wrong_key) {
			wrong_key = false;
			os_SetCursorPos(9, 0);
			os_PutStrFull("!");
		} else {
			os_ClrHomeFull();
			cmat_print_sel(m, prompt, row, col);
		}
		key = os_GetKey();
		switch (key) {
			case k_Right:
				col = abs(col + 1) % m->rowsize;
				break;
			case k_Left:
				col = abs((i16)col - 1) % m->rowsize;
				break;
			case k_Up:
				row = abs((i16)row - 1) % m->colsize;
				break;
			case k_Down:
				row = abs(row + 1) % m->colsize;
				break;
			case k_Enter:
				memset(input, 0, INPUT_SIZE);
				os_SetCursorPos(9, 0);
				os_GetTokenInput(":", input, INPUT_SIZE);
				cmindex(m, row, col) = parse_complex(input);
				break;
			case k_Del:
				isInput = false;
				break;
			case k_Clear:
				exit(0);
			default:
				wrong_key = true;
				break;
		}
	}

}

int main(void) {

	u16 msize = 2;
	memset(input, 0, INPUT_SIZE);
	os_ClrHomeFull();
	os_GetStringInput("number of variables: ", input, INPUT_SIZE);
	msize = parsei(input);

	cmat* cmat_in = cmat_create(msize, msize);
	cmat* cvec = cmat_create(1, msize);

	mat* mat_real = mat_create(msize * 2, msize * 2);
	mat* vec_real = mat_create(2, msize * 2);
	mat* mat_aug = mat_create((msize + 1) * 2, msize * 2);

	cmat_input(cmat_in, "input");
	cmat_input(cvec, "vector");

	cmat_real_representation(cmat_in, mat_real);
	cmat_real_representation(cvec, vec_real);
	mat_augment(mat_aug, mat_real, vec_real);
	mat_rref(mat_aug);
	mat_unaugment(mat_aug, vec_real);
	cmat_imag_representation(vec_real, cvec);

	os_ClrHomeFull();
	cmat_print(cvec, "imag output");

	cmat_destroy(cvec);
	cmat_destroy(cmat_in);
	mat_destroy(mat_real);
	mat_destroy(vec_real);
	mat_destroy(mat_aug);

	while (!os_GetCSC());
	return 0;

}
