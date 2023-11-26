/**************************************************************
 *
 *                     rgb.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     converts values from rgb to component video and vice
 *     versa for compression and decompression
 *
 **************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"
#include "cv_pic.h"
#include "rgb.h"
#include <mem.h>
#include "assert.h"
#include "math.h"

/********** rgb_to_cv ********
 *
 * converts rgb values to component-video values
 *
 * Parameters:
 *      Pnm_ppm picture: the picture whose values are to be converted
 *
 * Return: a ppm representation with component video values
 *
 * Expects
 *     picture not to be null
 * Notes:
 *      will CRE if picture is null
 ************************/
Pnm_ppm rgb_to_cv(Pnm_ppm picture) 
{
        assert(picture != NULL);
        /* make a new picture that will hold the CV values*/
        Pnm_ppm CV_pic = copy_pic(picture);

        /* convert RGB pixel array to a CV pixel array */
        const struct A2Methods_T *method = picture->methods;
        A2Methods_UArray2 pixels_cv = new_cv_pixels(picture->width, 
                                        picture->height, method);
        CV_pic->pixels = pixels_cv;

        /* convert RGB values to CV values */
        method->map_default(picture->pixels, applyCV, CV_pic);

        Pnm_ppmfree(&picture);

        return CV_pic;
}

/********** applyCV ********
 *
 * the function convert pixel values from rgb to cv
 *
 * Parameters:
 *      int i: number of columns 
 *      int j: number of rows
 *      A2Methods_UArray2 a: 2D array of methods 
 *      void *elem: pointer to current element
 *      void *cl: pointer to closure
 *
 * Return: none
 *
 * Expects
 *     none
 * Notes:
 *      apply function to use with map
 ************************/
void applyCV(int i, int j, A2Methods_UArray2 a, void *elem, void *cl)
{
        Pnm_ppm CV_pic = cl;
        const struct A2Methods_T *method_CV = CV_pic->methods;

        /* get RGB values from picture copy */
        Pnm_rgb picture_pixel = (Pnm_rgb)(method_CV->at(a, i, j));

        /* convert to floating point representation */
        float red_val = (float)(picture_pixel->red) / 
                        (float)CV_pic->denominator;
        float green_val = (float)(picture_pixel->green)/ 
                        (float)CV_pic->denominator;
        float blue_val = (float)(picture_pixel->blue) / 
                        (float)CV_pic->denominator;

        assert(red_val <= 1);
        assert(green_val <= 1);
        assert(blue_val <= 1);
        assert(red_val >= 0);
        assert(green_val >= 0);
        assert(blue_val >= 0);

        /* get the new location where we want to store the CV value */
        pic_cv newLoc = (pic_cv)(method_CV->at(CV_pic->pixels, i, j));

        /* put component video values into picture */
        newLoc->y = 0.299 * red_val + 0.587 * green_val + 0.114 * blue_val; 
        newLoc->pb = -0.168736 * red_val - 0.331264 * green_val + 0.5 * blue_val; 
        newLoc->pr = 0.5 * red_val - 0.418688 * green_val - 0.081312 * blue_val;

        assert(newLoc->y <= 1);
        assert(newLoc->pb <= .5);
        assert(newLoc->pr <= .5);
        assert(newLoc->y  >= 0);
        assert(newLoc->pb >= -.5);
        assert(newLoc->pr >= -.5);

        (void) elem;
}

/********** cv_to_rgb ********
 *
 * converts component-video values to rgb values
 *
 * Parameters:
 *      Pnm_ppm CV_pic: the picture whose values in component-video
 *
 * Return: a ppm representation with rgb values
 *
 * Expects
 *     CV_pic not to be null
 * Notes:
 *      will CRE if CV_pic is null
 ************************/
Pnm_ppm cv_to_rgb (Pnm_ppm CV_pic)
{
        /* make a new picture that will hold the RGB values*/
        Pnm_ppm RGB_pic = copy_pic(CV_pic);

        /* convert CV pixel array to a RGB pixel array */
        const struct A2Methods_T *method = CV_pic->methods;
        RGB_pic->denominator = 255;
        RGB_pic->pixels = method->new(CV_pic->width, 
                                            CV_pic->height, 
                                            sizeof(struct Pnm_rgb));

        /* convert RGB values to CV values */
        method->map_default(CV_pic->pixels, applyRGB, RGB_pic);

        Pnm_ppmfree(&CV_pic);

        return RGB_pic;
}

/********** applyRGB ********
 *
 * the function convert pixel values from cv to rgb
 *
 * Parameters:
 *      int i: number of columns 
 *      int j: number of rows
 *      A2Methods_UArray2 a: 2D array of methods 
 *      void *elem: pointer to current element
 *      void *cl: pointer to closure
 *
 * Return: none
 *
 * Expects
 *     none
 * Notes:
 *      apply function to use with map
 ************************/
void applyRGB(int i, int j, A2Methods_UArray2 a, void *elem, void *cl)
{
        Pnm_ppm RGB_pic = cl;
        const struct A2Methods_T *method = RGB_pic->methods;
        /* get CV values from picture copy */
        pic_cv picture_pixel = (pic_cv)(method->at(a, i, j));
        float y = picture_pixel->y;
        float pb = picture_pixel->pb;
        float pr = picture_pixel->pr;

        assert(y <= 1);
        assert(pb <= .5);
        assert(pr <= .5);
        assert(y >= 0);
        assert(pb >= -.5);
        assert(pr >= -.5);

        /* get the new location where we want to store the RGB value */
        Pnm_rgb newLoc = (Pnm_rgb)(method->at(RGB_pic->pixels, i, j));
        float r = (1.0 * y + 0.0      * pb + 1.402    * pr) * 255;
        float g = (1.0 * y - 0.344136 * pb - 0.714136 * pr) * 255;
        float b = (1.0 * y + 1.772    * pb + 0.0      * pr) * 255;

        r = fmax(0.0, fmin(255.0, r));
        g = fmax (0.0, fmin(255.0, g));
        b = fmax(0.0, fmin(255.0, b));

        /* put RGB values into picture */
        newLoc->red = (unsigned)(r);
        newLoc->green = (unsigned)(g);
        newLoc->blue = (unsigned)(b);

        (void) elem;
}

/********** copy_pic ********
 *
 * copies a ppm image into a new ppm image
 *
 * Parameters:
 *      Pnm_ppm picture: original image
 *
 * Return: 
 *      Pnm_ppm of the updated picture
 * Expects
 *     updated_pic != NULL
 * Notes:
 *      CRE if updated_pic == NULL
 ************************/
Pnm_ppm copy_pic(Pnm_ppm picture) 
{
        Pnm_ppm updated_pic;
        NEW(updated_pic);
        assert(updated_pic != NULL);

        updated_pic->denominator = picture->denominator;
        updated_pic->methods = picture->methods;
        updated_pic->pixels = picture->pixels;
        updated_pic->width = picture->width;
        updated_pic->height = picture->height;
        
        assert(updated_pic->pixels != NULL);
        return updated_pic;
}


