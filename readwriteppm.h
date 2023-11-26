/**************************************************************
 *
 *                     readwriteppm.h
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     header file for readwriteppm.c
 *
 **************************************************************/
#ifndef READWRITEPPM_INCLUDED
#define READWRITEPPM_INCLUDED

#include "pnm.h"

Pnm_ppm read_ppm(FILE *fp, A2Methods_T method);
void write_ppm(Pnm_ppm picture);
Pnm_ppm change_dim(Pnm_ppm old_pic, int width, int height);

#endif