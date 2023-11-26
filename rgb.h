/**************************************************************
 *
 *                     rgb.h
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     converts values from rgb to component video and vice
 *     versa for compression and decompression
 *
 **************************************************************/
#ifndef RGB_INCLUDED
#define RGB_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"
#include "cv_pic.h"

Pnm_ppm rgb_to_cv(Pnm_ppm picture);
void applyCV(int i, int j, A2Methods_UArray2 a, void *elem, void *cl);

Pnm_ppm cv_to_rgb (Pnm_ppm CV_pic);
void applyRGB(int i, int j, A2Methods_UArray2 a, void *elem, void *cl);

Pnm_ppm copy_pic(Pnm_ppm picture);

void printRGB(int i, int j, A2Methods_UArray2 a, void *elem, void *cl);
void printCV(int i, int j, A2Methods_UArray2 a, void *elem, void *cl);

#endif