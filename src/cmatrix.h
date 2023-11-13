#include "types.h"
#include "matrix.h"
#include "complex.h"

#ifndef CMATRIX_H
#define CMATRIX_H

#define MAX_CMATRIX_SIZE 10 * 10

// column-major matrix indexing
#define cmindex(c, row, col) c->d[(col) * c->colsize + (row)]

typedef struct {
	u16 rowsize;
	u16 colsize;
	complex d[MAX_CMATRIX_SIZE];
} cmat;

cmat* cmat_create(const u16 rowsize, const u16 colsize);
void cmat_destroy(cmat* c);
void cmat_print(const cmat* cmat, const char* name);
void cmat_print_sel(const cmat* m, const char* name, u16 sel_row, u16 sel_col);
void cmat_swap_row(cmat* c, const u16 rowa, const u16 rowb);
void cmat_swap_col(cmat* c, const u16 cola, const u16 colb);
void cmat_real_representation(cmat* c, mat* m);
void cmat_imag_representation(mat* m, cmat* c);

#undef MAX_CMATRIX_SIZE

#endif
