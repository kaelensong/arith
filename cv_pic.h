/**************************************************************
 *
 *                     cv_pic.h
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *      creates a new pixels 2D array for use in the Pnm_ppm 
 *      struct that can hold pb, pr, y values instead 
 *      of rgb values
 *
 **************************************************************/
#ifndef CV_PIC_INCLUDED
#define CV_PIC_INCLUDED
#include <stdio.h>
#include <stdint.h>

#include <except.h>

#include "a2methods.h"
#include "a2plain.h"


typedef struct pic_cv {
        float y, pb, pr;
} *pic_cv;

A2Methods_UArray2 new_cv_pixels(int width, int height, 
                                const struct A2Methods_T *methods);
void free_pixels(A2Methods_UArray2 *array, const struct A2Methods_T *methods);

#endif