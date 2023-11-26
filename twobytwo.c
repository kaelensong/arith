/**************************************************************
 *
 *                     twobytwo.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     converts chroma values and transforms dct in 2by2 blocks
 *     for compression and decompression of an image
 *
 **************************************************************/
#include "twobytwo.h"
#include "uarray.h"
#include "uarray2.h"
#include "assert.h"
#include "arith40.h"
#include "math.h"
#include "rgb.h"

/********** decomp_DCT_chroma ********
 *
 * converts a, b, c, d and pb, pr values for decompression
 * performs inverse DCT and chroma values
 *
 * Parameters:
 *      UArray_T array: the array of 2x2 blocks to be converted
 *      Pnm_ppm picture: the picture with the height, width,
 *                      and methods fields that we need
 *
 * Return: 
 *      a Pnm_ppm with the changed values  
 ************************/
Pnm_ppm decomp_DCT_chroma(UArray_T array, Pnm_ppm picture)
{
        Pnm_ppm CV_pic = copy_pic(picture);

        CV_pic->denominator = 255;

        const struct A2Methods_T *method = picture->methods;
        A2Methods_UArray2 pixels_cv = new_cv_pixels(picture->width, 
                                        picture->height, method);
        CV_pic->pixels = pixels_cv;

        for (int i = 0; i < UArray_length(array); i++) {
                block bl = UArray_at(array, i);

                float y1 = ((float)bl->a / 63 + (float)bl->b / 50 - 
                            (float)bl->c / 50 + (float)bl->d / 50);
                float y2 = ((float)bl->a / 63 - (float)bl->b / 50 + 
                            (float)bl->c / 50 - (float)bl->d / 50);
                float y3 = ((float)bl->a / 63 + (float)bl->b / 50 - 
                            (float)bl->c / 50 - (float)bl->d / 50);
                float y4 = ((float)bl->a / 63 + (float)bl->b / 50 + 
                            (float)bl->c / 50 + (float)bl->d / 50);

                /*Ensure that the calculated 'y' values are within the 
                valid range*/
                y1 = fmax(0.0, fmin(1.0, y1));
                y2 = fmax (0.0, fmin(1.0, y2));
                y3 = fmax(0.0, fmin(1.0, y3));
                y4 = fmax(0.0, fmin(1.0, y4));

                /* Pb and Pb -> chroma codes */
                float pb = Arith40_chroma_of_index(bl->avg_pb);
                float pr = Arith40_chroma_of_index(bl->avg_pr);
        
                assert(y1 <= 1);
                assert(y1  >= 0);
                assert(y3 <= 1);
                assert(y3  >= 0);
                assert(y4 <= 1);
                assert(y4 >= 0);
                assert(y2 <= 1);
                assert(y2  >= 0);
        
                pic_cv pixel1 = (pic_cv)method->at(CV_pic->pixels, bl->one.x, 
                                bl->one.y);
                pixel1->pb = pb;
                pixel1->pr = pr;
                pixel1->y = y1;
                
                pic_cv pixel2 = (pic_cv)method->at(CV_pic->pixels, bl->two.x, 
                                bl->two.y);
                pixel2->pb = pb;
                pixel2->pr = pr;
                pixel2->y = y2;

                pic_cv pixel3 = (pic_cv)method->at(CV_pic->pixels, bl->three.x,
                                bl->three.y);
                pixel3->pb = pb;
                pixel3->pr = pr;
                pixel3->y = y3;

                pic_cv pixel4 = (pic_cv)method->at(CV_pic->pixels, bl->four.x, 
                                bl->four.y);
                pixel4->pb = pb;
                pixel4->pr = pr;
                pixel4->y = y4;

        }

        /* return the CV pic */
        Pnm_ppmfree(&picture);
        return CV_pic;
}

/********** create_twoBytwos ********
 *
 * create an array that holds the 2x2 blocks
 * changed values from y to a, b, c, d by
 * performing DCT
 *
 * Parameters: the CV_pic with the values to be 
 * packed and converted
 *
 * Return: a 1D array that holds the blocks
 *
 * Expects
 *     array not to be null
 * Notes:
 *      will CRE is array is null or the picture's width or height
 *      is not even
 ************************/
UArray_T create_twoBytwos(Pnm_ppm CV_pic)
{
        int length = (CV_pic->width / 2) * (CV_pic->height / 2); 
        assert((CV_pic->width % 2 == 0) && (CV_pic->height % 2 == 0));

        UArray_T array = UArray_new(length, sizeof(struct block));
        assert(array != NULL);

        int index = 0;
        int h = CV_pic->height;
        int w = CV_pic->width;
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
                                                applyBlock(bl, CV_pic);
                                        }
                                }
                        }
                index++;
                }
        }
        return array;
}

/********** applyBlock ********
 *
 * converts floats to a, b, c, d, pb, and pr
 *
 * Parameters:
 *      block bl: block struct
 *      Pnm_ppm CV_pic: ppm pic with component video
 *      
 * Return: 
 *      none
 ************************/
void applyBlock(block bl, Pnm_ppm CV_pic)
{       

        const struct A2Methods_T *method = CV_pic->methods;

                int x1_val = bl->one.x;
                int y1_val = bl->one.y;

                pic_cv value1 = 
                (pic_cv)(method->at(CV_pic->pixels, x1_val, y1_val));

                float y1 = value1->y;
                float pb1 = value1->pb;
                float pr1 = value1->pr;
                int x2_val = bl->two.x;
                int y2_val = bl->two.y;

                pic_cv value2 = 
                (pic_cv)(method->at(CV_pic->pixels, x2_val, y2_val));
                float y2 = value2->y;
                float pb2 = value2->pb;
                float pr2 = value2->pr;
        
                int x3_val = bl->three.x;
                int y3_val = bl->three.y;
                pic_cv value3 = 
                (pic_cv)(method->at(CV_pic->pixels, x3_val, y3_val));
                float y3 = value3->y;
                float pb3 = value3->pb;
                float pr3 = value3->pr;
                
                int x4_val = bl->four.x;
                int y4_val = bl->four.y;

                pic_cv value4 = 
                (pic_cv)(method->at(CV_pic->pixels, x4_val, y4_val));
                float y4 = value4->y;
                float pb4 = value4->pb;
                float pr4 = value4->pr;
               
                float a = (y1 + y2 + y3 + y4) / 4.00;
                
                
                float scaled_a = a * 63.0;
                bl->a = round(scaled_a);
                assert(bl->a <= 63);
                
                float scaled_b = (((y4 + y3 - y2 - y1) / 4.00));
                float scaled_c = (((y4 - y3 + y2 - y1) / 4.00));
                float scaled_d = (((y4 - y3 - y2 + y1) / 4.00));
                if (scaled_b <= -0.3) scaled_b = -0.3;
                if (scaled_b >= 0.3) scaled_b = 0.3;
                if (scaled_c <= -0.3) scaled_c = -0.3;
                if (scaled_c >= 0.3) scaled_c = 0.3;
                if (scaled_d <= -0.3) scaled_d = -0.3;
                if (scaled_d >= 0.3) scaled_d = 0.3;
                
                int quantized_b = round(scaled_b * 50);
                int quantized_c = round(scaled_c * 50);
                int quantized_d = round(scaled_d * 50);

                /* Ensure the quantized values fall within the 5-bit range 
                (-15 to +15)*/
                if (quantized_b < -15) quantized_b = -15;
                if (quantized_b > 15) quantized_b = 15;
                if (quantized_c < -15) quantized_c = -15;
                if (quantized_c > 15) quantized_c = 15;
                if (quantized_d < -15) quantized_d = -15;
                if (quantized_d > 15) quantized_d = 15;
                bl->b = quantized_b;
                bl->c = quantized_c;
                bl->d = quantized_d;
               
                /* avg pb pr values to chroma values */
                float average_pb = (pb1 + pb2 + pb3 + pb4) / 4.00; 
                float average_pr = (pr1 + pr2 + pr3 + pr4) / 4.00;
                bl->avg_pb = Arith40_index_of_chroma(average_pb);
                bl->avg_pr = Arith40_index_of_chroma(average_pr);
                        assert(quantized_d <= 15);
}