/**************************************************************
 *
 *                     ppmdiff.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/25/2023
 *
 *    find the difference value between two images
 *
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include "assert.h"
#include "pnm.h"
#include "ppmdiff.h"
#include "math.h"

#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"


/********** main ********
 *
 * does the main operations for ppmdiff
 *
 * Parameters:
 *      int argc: number of input commands 
 *      char *argv[]: input commands
 *
 * Return: int
 *
 * Expects
 *     input from stdin or input file
 * Notes:
 *      file != NULL
 ************************/
int main(int argc, char *argv[]) 
{
        /* TODO: Maybe change?? default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain;
        assert(methods);

        //takes 2 arguments on command line
        if (argc < 3){
                fprintf(stderr, "Usage ppmdiff [picture 1] [picture 2]\n");
                return EXIT_FAILURE;
        }

        //open the first picture
        FILE *fp = open_or_fail(argv[1], "r");
        Pnm_ppm OG_pic;
        OG_pic = Pnm_ppmread(fp, methods);
        assert(OG_pic != NULL);
        fclose(fp); 

        //open the second picture
        FILE *fp1 = open_or_fail(argv[2], "r");
        Pnm_ppm edited_pic;
        edited_pic = Pnm_ppmread(fp1, methods);
        assert(edited_pic != NULL);
        fclose(fp1);

        if(edited_pic->width <= OG_pic->width - 2 && 
           edited_pic->width >= OG_pic->width + 2 && 
           edited_pic->height <= OG_pic->height - 2 && 
           edited_pic->height >= OG_pic->height + 2){
                fprintf(stderr, "Width or height differs by more than 1.\n");
                printf("1.0\n");
                return EXIT_FAILURE;
        }

        int smaller_width;
        int smaller_height;

        if(edited_pic->width > OG_pic->width){
                smaller_width = OG_pic->width;
        } else {
                smaller_width = edited_pic->width;
        }
        if(edited_pic->height > OG_pic->height){
                smaller_height = OG_pic->height;
        } else {
                smaller_height = edited_pic->height;
        }

        float sum = 0;
        
        for (int i = 0; i < smaller_width; i++) {
                for (int j = 0; j < smaller_height; j++) {
                        Pnm_rgb og_pic_pixel = 
                        (Pnm_rgb)(methods->at(OG_pic->pixels, i, j));
                        float OG_red_val = og_pic_pixel->red;
                        float OG_green_val = og_pic_pixel->green;
                        float OG_blue_val = og_pic_pixel->blue;

                        Pnm_rgb edited_pic_pixel = 
                        (Pnm_rgb)(methods->at(edited_pic->pixels, i, j));
                        float new_red_val = edited_pic_pixel->red;
                        float new_green_val = edited_pic_pixel->green;
                        float new_blue_val = edited_pic_pixel->blue;
                        
                        sum += ((OG_red_val/OG_pic->denominator - new_red_val/
                        edited_pic->denominator) * (OG_red_val/
                        OG_pic->denominator - new_red_val/
                        edited_pic->denominator)) + ((OG_green_val/
                        OG_pic->denominator - new_green_val/
                        edited_pic->denominator) * (OG_green_val/
                        OG_pic->denominator - new_green_val/
                        edited_pic->denominator)) + ((OG_blue_val/
                        OG_pic->denominator - new_blue_val/
                        edited_pic->denominator) * (OG_blue_val/
                        OG_pic->denominator - new_blue_val/
                        edited_pic->denominator));
                }
        }
        float denom = (3 * smaller_height * smaller_width);
        float percent = sqrt(sum/denom);
        printf("%.4f\n", percent);
}



/********** open_or_fail ********
 * 
 * tests that the given file can be opened, 
 * then opens the file if possible
 * Inputs: 
 *         char *filename: the filename
 *         char *mode: 
 *              
 * Return: the pointer to the opened file
 * 
 * Notes:
 *    Will exit failure if file cant be opened
 *    User will need to manually close the file
 ************************/
static FILE *open_or_fail(char *filename, char *mode) 
{
        FILE *fp = fopen(filename, mode);
        if(fp == NULL){
                int rc = errno;
                fprintf(stderr, "Could not open file %s with mode %s\n",
                        filename, mode);
                exit(rc);
        }
        return fp;
} 