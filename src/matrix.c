#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ti/screen.h>

#include "types.h"
#include "tiutil.h"

mat* mat_create(const u16 rowsize, const u16 colsize) {
	mat* m = malloc(sizeof(mat));
	assert(m != NULL);
	memset(m, 0, sizeof(mat));
	m->rowsize = rowsize;
	m->colsize = colsize;
	return m;
}

void mat_destroy(mat* m) {
	free(m);
}

void mat_print(const mat* m, const char* name) {
	//printf("%s:\n", name);
	for (u16 row = 0; row < m->colsize; row ++) {
		for (u16 col = 0; col < m->rowsize; col ++) {
			os_PutStrLine(ftos(mindex(m, row, col)));
			os_PutStrLine(" ");
			//printf("\t%.4f, ", mindex(mat, row, col));
		}
		os_NewLine();
	}
	os_NewLine();
}

void mat_swap_row(mat* m, const u16 rowa, const u16 rowb) {
	f64 temp;
	for (u16 col = 0; col < m->rowsize; col ++) {
		temp = mindex(m, rowa, col);
		mindex(m, rowa, col) = mindex(m, rowb, col);
		mindex(m, rowb, col) = temp;
	}
}

void mat_swap_col(mat* m, const u16 cola, const u16 colb) {
	f64 temp;
	for (u16 row = 0; row < m->colsize; row ++) {
		temp = mindex(m, cola, row);
		mindex(m, row, cola) = mindex(m, row, colb);
		mindex(m, row, colb) = temp;
	}
}

void mat_mul_row(mat* m, const u16 row, const f64 scalar) {
	for (u16 col = 0; col < m->rowsize; col ++)
		mindex(m, row, col) *= scalar;
}

void mat_div_row(mat* m, const u16 row, const f64 divisor) {
	assert(divisor != 0);
	for (u16 col = 0; col < m->rowsize; col ++)
		mindex(m, row, col) /= divisor;
}

void mat_mul_col(mat* m, const u16 col, const f64 scalar) {
	for (u16 row = 0; row < m->colsize; row ++)
		mindex(m, row, col) *= scalar;
}

void mat_div_col(mat* m, const u16 col, const f64 divisor) {
	assert(divisor != 0);
	for (u16 row = 0; row < m->colsize; row ++)
		mindex(m, row, col) /= divisor;
}

void mat_add_row(mat* m, const u16 dst, const u16 src, const f64 scalar) {
	assert(dst != src);
	assert(!(dst >= m->rowsize));
	assert(!(src >= m->rowsize));
	assert(!(dst < 0));
	assert(!(src < 0));
	for (u16 col = 0; col < m->rowsize; col ++) {
		mindex(m, dst, col) += scalar * mindex(m, src, col);
	}
}

void mat_add_col(mat* m, const u16 dst, const u16 src, const f64 scalar) {
	assert(dst != src);
	assert(!(dst >= m->rowsize));
	assert(!(src >= m->rowsize));
	assert(!(dst < 0));
	assert(!(src < 0));
	for (u16 row = 0; row < m->colsize; row ++) {
		mindex(m, dst, src) += scalar * mindex(m, row, src);
	}
}

void mat_augment(mat* aug, mat* m, mat* v) {
	assert(aug->rowsize == m->rowsize + v->rowsize);
	assert(aug->colsize == m->colsize);
	assert(aug->colsize == v->colsize);
	const u16 mlen = m->rowsize * m->colsize;
	const u16 vlen = v->rowsize * v->colsize;
	memcpy(aug->d       , m->d, mlen * sizeof(f64));
	memcpy(aug->d + mlen, v->d, vlen * sizeof(f64));
}

void mat_unaugment(mat* aug, mat* v) {
	assert(aug->colsize == v->colsize);
	const u16 mlen = aug->colsize * aug->colsize;
	const u16 vlen = v->rowsize * v->colsize;
	memcpy(v->d, aug->d + mlen, vlen * sizeof(f64));
}

void mat_rref(mat* aug) {

	f64 divisor;
	f64 scalar;
	u16 colsize = aug->colsize;

	for (u16 col = 0; col < colsize; col ++) {

		u16 rowpush = 0;
		while (mindex(aug, col + rowpush, col) == 0.0f)
			rowpush ++;
		if (rowpush > 0)
			mat_swap_row(aug, col, col + rowpush);
		divisor = mindex(aug, col, col);
		mat_div_row(aug, col, divisor);

		for (u16 row = col + 1; row < colsize; row ++) {
			if (row >= colsize) break;
			scalar = -mindex(aug, row, col);
			mat_add_row(aug, row, col, scalar);
		}
	}

	for (u16 col = 0; col < colsize; col ++) {
		for (u16 row = 0; row < col; row ++) {
			if (row > colsize) break;
			scalar = -mindex(aug, row, col);
			mat_add_row(aug, row, col, scalar);
		}
	}

}

