/**************************************************************
 *
 *                     towbytwo.h
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/25/2023
 *
 *     converts chroma values and transforms dct in 2by2 blocks
 *     for compression and decompression of an image
 *
 **************************************************************/
#ifndef TWOBYTWO_INCLUDED
#define TWOBYTWO_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#include "except.h"
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"
#include "cv_pic.h"
#include "uarray.h"
#include "uarray2.h"
struct elem {
        int x;
        int y;
};


typedef struct block {
        struct elem one, two, three, four;
        unsigned a;
        signed b, c, d;
        unsigned avg_pb, avg_pr;
} *block;

UArray_T create_twoBytwos(Pnm_ppm CV_pic);

Pnm_ppm decomp_DCT_chroma(UArray_T array, Pnm_ppm picture);
void apply_twobytwo(int i, int j, UArray2_T a, void *elem, void *cl);

void applyBlock(block bl, Pnm_ppm CV_pic);

void printArray(UArray_T array);

#endif