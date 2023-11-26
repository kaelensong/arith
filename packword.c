/**************************************************************
 *
 *                     packword.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     summary packs and unpacks 32-bit words for compression
 *     and compression
 *
 **************************************************************/
#include "packword.h"

/********** pack_word ********
 *
 * packs and puts 32-bit words into an array
 *
 * Parameters: a UArray containing the 2x2 blocks
 *      
 *
 * Return: a UArray of 32-bit words made from the 2x2 blocks
 *
 * Expects
 *     the arrays not to be null
 * Notes: will CRE if arrays are null
 *      
 ************************/
UArray_T pack_word(UArray_T array)
{
        UArray_T word_array = UArray_new(UArray_length(array), 
                                sizeof(uint32_t));
        assert(word_array != NULL && array != NULL);
        for (int length = 0; length < UArray_length(array); length++) {
                uint32_t word = 0;
                block bl = (block)UArray_at(array, length);
                
                /* a: 9 bits, lsb: 23 - unsigned scaled int */
                uint8_t a = bl->a;
                word = Bitpack_newu(word, 6, 26, a);
                
                /* b: 5 bits, lsb: 18 - signed scaled int */
                int8_t b = bl->b;
                word = Bitpack_news(word, 6, 20, b);

                /* c: 5 bits, lsb: 13 - signed scaled int*/
                int8_t c = bl->c;
                word = Bitpack_news(word, 6, 14, c);

                /* d: 5 bits, lsb: 8 - signed scaled int */
                int8_t d = bl->d;
                word = Bitpack_news(word, 6, 8, d);

                /* pb: 4 bits, lsb: 4 - unsigned index */
                uint8_t pb = bl->avg_pb;
                word = Bitpack_newu(word, 4, 4, pb);

                /* pr: 4 bits, lsb: 0 - unsigned index */
                uint8_t pr = bl->avg_pr;
                word = Bitpack_newu(word, 4, 0, pr);
                uint32_t *index = (uint32_t*)UArray_at(word_array, length);
                *index = word;
        }

        return word_array;
}


/********** unpack_word ********
 *
 * unpacks 32-bit words into local variables, to be stored in an array
 *
 * Parameters: an array containing the words 
 *      
 *
 * Return: the array containing the local variables
 *
 * Expects
 *     the arrays not to be null
 * Notes:
 *      will CRE if arrays are null
 ************************/
UArray_T unpack_word(UArray_T words_a, Pnm_ppm picture)
{
        assert((picture->width * picture->height) % 4 == 0);

        UArray_T array = create_blocks(picture);
        assert(array != NULL && words_a != NULL);
        /* read unpacked words into array */
        for (int i = 0; i < UArray_length(words_a); i++){
                /* unpack each word */
                uint32_t curr_word = *(uint32_t*)UArray_at(words_a, i);
                /* a: 9 bits, lsb: 23 - unsigned scaled int */
                unsigned a = Bitpack_getu(curr_word, 6, 26);
                /* b: 5 bits, lsb: 18 - signed scaled int */
                signed b = Bitpack_gets(curr_word, 6, 20);
                /* c: 5 bits, lsb: 13 - signed scaled int*/
                signed c = Bitpack_gets(curr_word, 6, 14);
                /* d: 5 bits, lsb: 8 - signed scaled int */
                signed d = Bitpack_gets(curr_word, 6, 8);
                /* pb: 4 bits, lsb: 4 - unsigned index */
                unsigned pb = Bitpack_getu(curr_word, 4, 4);
                /* pr: 4 bits, lsb: 0 - unsigned index */
                unsigned pr = Bitpack_getu(curr_word, 4, 0);

                block bl = (block)UArray_at(array, i);
                bl->a = a;
                bl->b = b;
                bl->c = c;
                bl->d = d;
                bl->avg_pb = pb;
                bl->avg_pr = pr;

        }

        return array;
}

/********** create_blocks ********
 *
 * calculates the indicies for each pixel
 *
 * Parameters: 
 *      Pnm_ppm picture: picture where the 
 *      decompressed image will be stored
 *
 * Return: the array containing the indicies
 *
 * Expects
 *     the array not to be null
 * Notes:
 *      will CRE if array is null
 ************************/
UArray_T create_blocks(Pnm_ppm picture)
{
        int length = (picture->width / 2) * (picture->height / 2); 
        assert((picture->width % 2 == 0) && (picture->height % 2 == 0));

        UArray_T array = UArray_new(length, sizeof(struct block));
        assert(array != NULL);

        int index = 0;
        int h = picture->height;
        int w = picture->width;
        for (int h_of_pic = 0; h_of_pic < h; h_of_pic += 2) {
                for (int w_of_pic = 0; w_of_pic < w; w_of_pic += 2) {
                int len = 4; /*cells have 4 elems*/

                /* Calculate the coordinates of the upper left corner of the 
                current block */
                int i0 = w_of_pic;
                int j0 = h_of_pic;

                block bl = (block)UArray_at(array, index);
                        for (int cell = 0; cell < len; cell++) {
                                int col = i0 + cell % 2;
                                int row = j0 + cell / 2;

                                if (row < h_of_pic + 2 && col < w_of_pic + 2) {
                                        if (cell == 0) {
                                                bl->one.x = col;
                                                bl->one.y = row;
                                        } else if (cell == 1) {
                                                bl->two.x = col;
                                                bl->two.y = row;
                                        } else if (cell == 2) {
                                                bl->three.x = col;
                                                bl->three.y = row;
                                        } else if (cell == 3) {
                                                bl->four.x = col;
                                                bl->four.y = row;
                                        }
                                }
                        }
                index++;
                }
        }
        return array;
}