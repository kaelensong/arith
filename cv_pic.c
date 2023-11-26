/**************************************************************
 *
 *                     cv_pic.c
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
#include <stdio.h>
#include <stdint.h>
#include "cv_pic.h"
#include "assert.h"
#include <except.h>
#include <a2methods.h>

/********** new_cv_pixels ********
 *
 * creates a new pixels 2D array that can hold pb, pr, y
 * values instead of rgb values
 *
 * Parameters:
 *      int width: the desired with of the array
 *      int height: the desired height of the array
 *      const struct A2Methods_T *methods: method
 *              with which to perform on the array
 *
 * Return: the new 2D pixels array
 *
 * Expects
 *     none
 ************************/
A2Methods_UArray2 new_cv_pixels(int width, int height, const struct 
                                A2Methods_T *methods)
{
        A2Methods_UArray2 new_pixels = methods->new_with_blocksize(width, 
                                        height, sizeof(struct pic_cv), 2);
        return new_pixels;
}


/********** free_pixels ********
 *
 * Frees the pixels array
 *
 * Parameters: 
 * 
 * A2Methods_UArray2 *array: a pointer to the array to be freed 
 * const struct A2Methods_T *methods: methods to be performed
 *                              on the 2D array     
 *
 * Return: none
 *
 * Expects
 *     array not to be null
 * Notes:
 *      will CRE if array is null
 ************************/
void free_pixels(A2Methods_UArray2 *array, const struct A2Methods_T *methods)
{
        assert(array != NULL);
        methods->free(array);
}