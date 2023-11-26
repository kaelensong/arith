/**************************************************************
 *
 *                     compress40.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     performs the operations of both compression and decompression
 *
 **************************************************************/
#include "compress40.h"
#include "readwriteppm.h"
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"
#include "rgb.h"
#include "assert.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "twobytwo.h"
#include "packword.h"
#include "readCompress.h"
#include "uarray.h"
#include "uarray2.h"

/********** compress40 ********
 *
 * performs compress operations on a ppm image
 *
 * Parameters:
 *      FILE *input: file containing ppm image
 *
 * Return: 
 *      none
 * Expects
 *     file not to be null, methods n
 * o be nullNotes:
 *      
 **CRE if methods == null********* t*************/
extern void compress40  (FILE *input)
{
        A2Methods_T methods = uarray2_methods_plain;
 
        assert(methods);

        /* reads the ppm */
        Pnm_ppm picture = read_ppm(input, methods);

        /* convert RGB to component video */
        picture = rgb_to_cv(picture);

        /* perform DCT and chroma transformations*/
        UArray_T twoBytwo_array = create_twoBytwos(picture);

        /* pack the image into a 32-bit word */
        UArray_T words_a = pack_word(twoBytwo_array);
        
        /* write the compressed image to stdout */
        write_compress(picture->width, picture->height, words_a);

        /* free memory */
        UArray_free(&twoBytwo_array);
        UArray_free(&words_a);
        Pnm_ppmfree(&picture);
}

/********** decompress40 ********
 * *
 * performs decompress operations on a ppm image
 *
 * Parameters:
 *      FILE *input: file containing ppm image
 *
 * Return: 
 *      none
 * Expects
 *     file not to be null, methods not to be null
 * Notes:
 *      CRE if methods == null************************/
extern void decompress40(FILE *input)
{ 
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);

        Pnm_ppm picture = read_compress(input, methods);

        UArray_T words_a = make_word_array(input, picture);

        UArray_T twoBytwo_array = unpack_word(words_a, picture);

        picture = decomp_DCT_chroma(twoBytwo_array, picture);        

        picture = cv_to_rgb(picture);

        write_ppm(picture); 

        UArray_free(&words_a);
        UArray_free(&twoBytwo_array);
        Pnm_ppmfree(&picture);
}