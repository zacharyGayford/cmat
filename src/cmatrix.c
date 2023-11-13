#include "cmatrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ti/screen.h>

#include "types.h"
#include "tiutil.h"

cmat* cmat_create(const u16 rowsize, const u16 colsize) {
	cmat* m = malloc(sizeof(cmat));
	assert(m != NULL);
	memset(m, 0, sizeof(cmat));
	m->rowsize = rowsize;
	m->colsize = colsize;
	return m;
}

void cmat_destroy(cmat* m) {
	free(m);
}

void cmat_print(const cmat* m, const char* name) {
	os_ClrHomeFull();
	os_PutStrFull(name);
	os_PutStrFull("=");
	os_NewLine();
	os_FontSelect(os_SmallFont);
	os_SetDrawFGColor(os_DrawFGColor);
	os_SetDrawBGColor(os_DrawBGColor);
	const u16 font_width = os_FontGetWidth("_");
	const u16 font_height = os_FontGetHeight();
	u16 draw_row = 5 * font_height;
	u16 draw_col = font_width;
	for (u16 row = 0; row < m->colsize; row ++) {
		for (u16 col = 0; col < m->rowsize; col ++) {
			complex c = cmindex(m, row, col);
			draw_col = os_FontDrawText(ftos(c.real), draw_col, draw_row);
			draw_col = os_FontDrawText("+", draw_col, draw_row);
			draw_col = os_FontDrawText(ftos(c.imag), draw_col, draw_row);
			draw_col = os_FontDrawText("i,", draw_col, draw_row);
			draw_col += font_width;
		}
		draw_col = font_width;
		draw_row += font_height;
	}
	os_NewLine();
}

void cmat_print_sel(const cmat* m, const char* name, u16 sel_row, u16 sel_col) {
	os_ClrHomeFull();
	os_PutStrFull(name);
	os_PutStrFull("=");
	os_FontSelect(os_SmallFont);
	const u16 font_width = os_FontGetWidth("_");
	const u16 font_height = os_FontGetHeight();
	u16 draw_row = 5 * font_height;
	u16 draw_col = font_width;
	for (u16 row = 0; row < m->colsize; row ++) {
		for (u16 col = 0; col < m->rowsize; col ++) {
			if (row == sel_row && col == sel_col) {
				os_SetDrawFGColor(os_TextBGColor);
				os_SetDrawBGColor(os_TextFGColor);
			} else {
				os_SetDrawFGColor(os_TextFGColor);
				os_SetDrawBGColor(os_TextBGColor);
			}
			complex c = cmindex(m, row, col);
			draw_col = os_FontDrawText(ftos(c.real), draw_col, draw_row);
			draw_col = os_FontDrawText("+", draw_col, draw_row);
			draw_col = os_FontDrawText(ftos(c.imag), draw_col, draw_row);
			draw_col = os_FontDrawText("i,", draw_col, draw_row);
			draw_col += font_width;
		}
		draw_col = font_width;
		draw_row += font_height;
	}
}

void cmat_swap_row(cmat* m, const u16 rowa, const u16 rowb) {
	complex temp;
	for (u16 col = 0; col < m->rowsize; col ++) {
		temp = cmindex(m, rowa, col);
		cmindex(m, rowa, col) = cmindex(m, rowb, col);
		cmindex(m, rowb, col) = temp;
	}
}

void cmat_swap_col(cmat* m, const u16 cola, const u16 colb) {
	complex temp;
	for (u16 row = 0; row < m->colsize; row ++) {
		temp = cmindex(m, cola, row);
		cmindex(m, row, cola) = cmindex(m, row, colb);
		cmindex(m, row, colb) = temp;
	}
}

void cmat_real_representation(cmat* c, mat* m) {
	assert(2 * c->rowsize == m->rowsize);
	assert(2 * c->colsize == m->colsize);
	for (u16 row = 0; row < c->colsize; row ++) {
		for (u16 col = 0; col < c->rowsize; col ++) {
			complex n = cmindex(c, row, col);
			mindex(m, 2 * row + 0, 2 * col + 0) =  n.real;
			mindex(m, 2 * row + 1, 2 * col + 1) =  n.real;
			mindex(m, 2 * row + 1, 2 * col + 0) =  n.imag;
			mindex(m, 2 * row + 0, 2 * col + 1) = -n.imag;
		}
	}
}

void cmat_imag_representation(mat* m, cmat* c) {
	assert(2 * c->rowsize == m->rowsize);
	assert(2 * c->colsize == m->colsize);
	for (u16 row = 0; row < c->colsize; row ++) {
		for (u16 col = 0; col < c->rowsize; col ++) {
			cmindex(c, row, col).real =  mindex(m, 2 * row, 2 * col);
			cmindex(c, row, col).imag = -mindex(m, 2 * row, 2 * col + 1);
		}
	}
}
