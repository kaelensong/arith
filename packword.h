/**************************************************************
 *
 *                     packword.h
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     summary packs and unpacks 32-bit words for compression
 *     and compression
 * 
 **************************************************************/
#ifndef PACKWORD_INCLUDED
#define PACKWORD_INCLUDED
#include "uarray.h"
#include "pnm.h"
#include "twobytwo.h"
#include "bitpack.h"
#include "assert.h"

UArray_T pack_word(UArray_T array);
UArray_T unpack_word(UArray_T words_a, Pnm_ppm picture);
UArray_T create_blocks(Pnm_ppm picture);

#endif
