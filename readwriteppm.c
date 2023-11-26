/**************************************************************
 *
 *                     readwriteppm.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     reads a ppm image from a file and writes a ppm image to stdout
 *
 **************************************************************/
#include "pnm.h"
#include "readwriteppm.h"
#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mem.h>

/********** read_ppm ********
 *
 * read a ppm image from a file and store the image in a Pnm_ppm struct
 * trims the dimensions of the picture to be even
 *
 * Parameters: FILE *fp: the file where the image is stored
 * A2Methods_T method: the methods to store in in the Pnm_ppm struct  
 *
 * Return: the picture, stored in the Pnm_ppm struct
 *
 * Expects
 *      the picture not to be null
 * Notes:
 *      will CRE if the picture is null
 ************************/
Pnm_ppm read_ppm(FILE *fp, A2Methods_T method)
{
        Pnm_ppm picture;
        picture = Pnm_ppmread(fp, method);
        assert(picture != NULL);

        int height = picture->height;
        int width = picture->width;
        bool trim_pic = false;
        /* check width and height are even numbers */
        if (picture->height % 2 != 0) {
                height = picture->height - 1;
                trim_pic = true;
        } 
        if (picture->width % 2 != 0) {
                width = picture->width - 1;
                trim_pic = true;
        }
        if (trim_pic) {
                picture = change_dim(picture, width, height);
        }
        return picture;
}

/********** change_dim ********
 *
 * changes the dimensions of a picture to be even
 *
 * Parameters: 
        * Pnm_ppm old_pic: the old picture, whose 
        *               dimensions must be changed
        * int width: the new even width 
        * int height: the new even height
 *      
 * Return: the new picture with even height and width
 *
 * Expects
 *     the new pic not to be null
 * Notes: 
 *      will CRE if the pic is null
 ************************/
Pnm_ppm change_dim(Pnm_ppm old_pic, int width, int height){
        Pnm_ppm new_pic;
        NEW(new_pic);
        assert(new_pic != NULL);
        new_pic->denominator = old_pic->denominator;
        const struct A2Methods_T *methods = old_pic->methods;
        new_pic->methods = old_pic->methods;
        new_pic->pixels = methods->new(width, height, sizeof(struct Pnm_rgb));
        new_pic->height = height;
        new_pic->width = width;

        for (int i = 0; i < width; i++){
                for (int j = 0; j < height; j++){                        
                        Pnm_rgb old_elem = (Pnm_rgb)methods->
                                           at(old_pic->pixels, i, j);
                        *(Pnm_rgb)methods->at(new_pic->pixels, i, j) 
                                                        = *old_elem;
                }
        }
        Pnm_ppmfree(&old_pic);
        return new_pic;
}


/********** write_ppm ********
 *
 * writes the ppm to standard output
 * 
 * Parameters: 
 *      Pnm_ppm picture: the picture to be written
 *
 * Return: 
 *      nothing directly   
 ************************/
void write_ppm(Pnm_ppm picture)
{
        Pnm_ppmwrite(stdout, picture);
}