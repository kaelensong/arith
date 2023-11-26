/**************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     contains the code for the function of the bitpack interface that 
 *     manipulates 64 bit signed and unsigned integers
 *
 **************************************************************/
#include "except.h"
#include "bitpack.h"
#include "math.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <assert.h>

Except_T Bitpack_Overflow = { "Overflow packing bits" };


/********** Bitpack_fitsu ********
 *
 * returns if an unsigned integer fits in bits of width
 *
 * Parameters:
 *      uint64_t n: 64 bit unsigned int
 *      unsigned width: number of bits in space
 * Return: 
 *      bool
 * Expects
 *     width to be less than 64
 * Notes:
 *      none
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width) {
    if (width >= 64) {
        return true; 
    }
    uint64_t max_val = (1ULL << width) - 1;
    return n <= max_val;
}

/********** Bitpack_fitss ********
 *
 * returns if a signed integer fits in bits of width
 *
 * Parameters:
 *      int64_t n: 64 bit signed int
 *      unsigned width: number of bits in space
 * Return: 
 *      bool
 * Expects
 *     width to be less than 64
 * Notes:
 *      none
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width) {
    if (width >= 64) {
        return true;
    }
    int64_t max_positive_val = (1ULL << (width - 1)) - 1;
    int64_t min_negative_val = -(1ULL << (width - 1));
    return n >= min_negative_val && n <= max_positive_val;
}


/********** Bitpack_getu ********
 *
 * gets a value in an unsigned word
 *
 * Parameters:
 *      uint64_t word: unsigned value int
 *      unsigned width: width of desired value
 *      unsigned lsb: least significant bit
 *
 * Return: 
 *      uint64_t value of width
 * Expects
 *     width <= 64, width + lsb <= 64
 * Notes:
 *      CRE if width > 64 or width + lsb > 64
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        /* checked runtime error if 0 <= w <= 64 */
        assert(width <= 64);
        /* checked runtime error if w + lsb <= 64 */
        assert(width + lsb <= 64);

        uint64_t y = ~0ULL;
        y = y >> (64 - width);
        y = y << (lsb);

        word = word & (y);
        word = word >> (lsb);
        return word;       
}

/********** Bitpack_gets ********
 *
 * gets a value in a signed word
 *
 * Parameters:
 *      uint64_t word: unsigned value/ int
 *      unsigned width: width of desired value
 *      unsigned lsb: least significant bit
 *
 * Return: 
 *      int64_t value of width
 * Expects
 *     width <= 64, width + lsb <= 64
 * Notes:
 *      CRE if width > 64 or width + lsb > 64
 ************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) 
{

    assert(width <= 64);
    assert(width + lsb <= 64);

    uint64_t y = 0ULL;
        y = y >> (64 - width);
        y = y << (lsb);

    uint64_t shifted = word << (64 - (width + lsb));

    int64_t s_word = (int64_t)(shifted) >> (64 - width);
    s_word = s_word | (y);

    return s_word;
}

/********** Bitpack_newu ********
 *
 * replaces value at width with a new value in unsigned word
 *
 * Parameters:
 *      uint64_t word: unsigned value/ int
 *      unsigned width: width of desired value
 *      unsigned lsb: least significant bit
 *      uint64_t value: new value
 *
 * Return: 
 *      uint64_t word with new value
 * Expects
 *     width <= 64, width + lsb <= 64
 * Notes:
 *      CRE if width > 64 or width + lsb > 64
 ************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);
        uint64_t y = ~0ULL;
        y = y >> (64 - width);
        y = y << lsb;
        y = ~y;
        word = y & (word);
        value = value << (lsb);
        word = word | (value);
        return word;
}

/********** Bitpack_news ********
 *
 * replaces value at width with a new value in signed word
 *
 * Parameters:
 *      uint64_t word: unsigned value/ int
 *      unsigned width: width of desired value
 *      unsigned lsb: least significant bit
 *      int64_t value: new value
 *
 * Return: 
 *      uint64_t word with new signed value
 * Expects
 *     width <= 64, width + lsb <= 64
 * Notes:
 *      CRE if width > 64 or width + lsb > 64
 ************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                        int64_t value)
{
    assert(width <= 64);
    assert(width + lsb <= 64);

    // Calculate the mask for the specified width and lsb
    uint64_t mask = ((1ULL << width) - 1) << lsb;

    // Clear the bits in the specified range
    word = word & ~mask;

    // Extract the relevant bits from the signed value
    uint64_t extractedValue = (uint64_t)(value) & ((1ULL << width) - 1);

    // Shift the extracted value to the correct position
    extractedValue = extractedValue << lsb;

    // Merge the extracted value with the word
    word = word | extractedValue;

    return word;
}

