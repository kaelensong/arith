/**************************************************************
 *
 *                     readCompress.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     reads a compressed image and writes a compressed image
 *
 **************************************************************/
#include "readCompress.h"
#include "mem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bitpack.h"

/********** write_compress ********
 *
 * writes a compressed image to stdout
 *
 * Parameters:
 *      int width: width of ppm image
 *      int height: hieght of ppm image
 *      UArray_T words: array of 32 bit words
 *
 * Return: none (void function)
 *
 * Expects
 *     UArray_T words != NULL
 * Notes:
 *      will CRE if array is null
 ************************/

void write_compress(int width, int height, UArray_T words)
{
    assert(words != NULL);
    printf("COMP40 Compressed image format 2\n%u %u\n", width, height);

    for (int i = 0; i < UArray_length(words); ++i) {
        uint32_t *codeWord = (uint32_t*)UArray_at(words, i);

        for (int j = 3; j >= 0; j--) {
            uint8_t byte = Bitpack_getu(*codeWord, 8, j * 8);
            putchar(byte);
        }
    }
}

/********** read_compress ********
 *
 * read a compressed image
 *
 * Parameters:
 *      FILE *input: compressed file 
 *      A2Methods_T method: methods for UArray2b_T
 * Return: 
 *      Pnm_ppm
 * Expects
 *     file not to be Null
 * Notes:
 *      CRE if file is null
 ************************/
Pnm_ppm read_compress(FILE *input, A2Methods_T method) 
{
        Pnm_ppm picture;
        NEW(picture);
        assert(picture != NULL);
        unsigned height , width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                        &width, &height);

        assert(read == 2);
        int c = getc(input); 
        assert(c == '\n');

        picture->height = height;
        picture->width = width;
        picture->denominator = 255;
        picture->methods = method;
        picture->pixels = method->new(width, height, sizeof(struct Pnm_rgb));

        return picture;

}


/********** make_word_array ********
 *
 * reads is the 32 bit words of a compressed image
 *
 * Parameters:
 *      FILE *input: compressed file 
 *      Pnm_ppm picture: ppm image 
 * Return: 
 *      UArray_T of 32 bit words
 * Expects
 *     file not to be Null
 * Notes:
 *      CRE if file is null
 ************************/

UArray_T make_word_array(FILE *input, Pnm_ppm picture) 
{
    assert(input != NULL);
    int length = (picture->width / 2) * (picture->height / 2); 
    UArray_T words_a = UArray_new(length, sizeof(uint32_t));

    for (int i = 0; i < length; i++) {

    uint32_t curr_word = 0;
        for (int j = 3; j >= 0; j--) {
            int byte = getc(input);
            assert(byte != EOF); 
            curr_word = Bitpack_newu(curr_word, 8, j * 8, byte);
            
        }

        uint32_t *index = (uint32_t*)UArray_at(words_a, i);
        
        *index = curr_word;
    }
    
    return words_a;
}


