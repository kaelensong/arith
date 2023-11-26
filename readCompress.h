/**************************************************************
 *
 *                     readCompress.h
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     header for readCompress.c
 *
 **************************************************************/
#ifndef READCOMPRESS_INCLUDED
#define READCOMPRESS_INCLUDED
#include "uarray.h"
#include "a2methods.h"
#include "pnm.h"
#include "assert.h"
#include <stdio.h>


void write_compress(int width, int height, UArray_T words);
Pnm_ppm read_compress(FILE *input, A2Methods_T method);
UArray_T make_word_array(FILE *input, Pnm_ppm picture);

#endif