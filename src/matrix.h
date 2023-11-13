#include "types.h"

#ifndef MATRIX_H
#define MATRIX_H

#define MAX_MATRIX_SIZE 20 * 20

// column-major matrix indexing
#define mindex(mat, row, col) mat->d[(col) * mat->colsize + (row)]

typedef struct {
	u16 rowsize;
	u16 colsize;
	f64 d[MAX_MATRIX_SIZE];
} mat;

mat* mat_create(const u16 rowsize, const u16 colsize);
void mat_destroy(mat* m);
void mat_print(const mat* mat, const char* name);
void mat_swap_row(mat* m, const u16 rowa, const u16 rowb);
void mat_swap_col(mat* m, const u16 cola, const u16 colb);
void mat_mul_row(mat* m, const u16 row, const f64 scalar);
void mat_div_row(mat* m, const u16 row, const f64 divisor);
void mat_mul_col(mat* m, const u16 col, const f64 scalar);
void mat_div_col(mat* m, const u16 col, const f64 divisor);
void mat_add_row(mat* m, const u16 dst, const u16 src, const f64 scalar);
void mat_add_col(mat* m, const u16 dst, const u16 src, const f64 scalar);
void mat_augment(mat* aug, mat* m, mat* v);
void mat_unaugment(mat* aug, mat* v);
void mat_rref(mat* aug);

#undef MAX_MATRIX_SIZE

#endif
