/**************************************************************
 *
 *                     bitpack_testing.c
 *
 *     Assignment: Arith
 *     Authors:  Kaelen Song (ksong05), Esther Khair (ekhair01) 
 *     Date:     10/26/2023
 *
 *     Unit testing for bitpack functions
 *
 **************************************************************/
#include "bitpack.h"
#include "assert.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

/********** Bitpack_fits_u ********
 *
 * Test Bitpack_fitsu
 *
 * Parameters: none
 *    
 * Return: none
 *
 * Expects: none
 *     
 * Notes: none
 *      
 ************************/
void Bitpack_fits_u() 
{
        uint64_t n = 34;
        unsigned width = 6;
        bool yes = Bitpack_fitsu(n, width);
        assert(yes == true);

        n = 34;
        width = 5;
        assert(Bitpack_fitsu(n, width) == false);
}

/********** Bitpack_fits_s ********
 *
 * Test Bitpack_fitss
 *
 * Parameters: none
 *    
 * Return: none
 *
 * Expects: none
 *     
 * Notes: none
 *      
 ************************/
void Bitpack_fits_s() 
{
        int64_t n = 5;
        unsigned width = 4;
        assert(Bitpack_fitss(n, width) == true);
        
        n = 4;
        width = 3;
        assert(Bitpack_fitss(n, width) == false);

        n = -4;
        width = 3;
        assert(Bitpack_fitss(n, width) == true);
}

/********** Bitpackgetu ********
 *
 * Test Bitpack_getu
 *
 * Parameters: none
 *
 * Return: none
 *
 * Expects
 *     none
 * Notes: 
 *      none
 ************************/
void Bitpackgetu() 
{
        uint64_t word = 0x3f4;
        uint64_t num = Bitpack_getu(word, 6, 2);
        fprintf(stderr, "num: %ld\n", num);
        assert(num == 61);
}
 
/********** Bitpackgets ********
 *
 * Test Bitpack_gets
 *
 * Parameters: none
 *
 * Return: none
 *
 * Expects
 *     none
 * Notes: 
 *      none
 ************************/
void Bitpackgets() 
{
        uint64_t word = 0x3f4;
        int64_t num = Bitpack_gets(word, 6, 2);
        fprintf(stderr, "num: %ld\n", num);
        assert(num == -3);


         word = -3;
         num = Bitpack_gets(word, 4, 2);
        fprintf(stderr, "hi num: %ld\n", num);
}

/********** Bitpacknewu ********
 *
 * Test Bitpack_newu
 *
 * Parameters:
 *      none
 * Return: 
 *      none
 * Expects
 *     none
 * Notes:
 *      none
 ************************/
void Bitpacknewu() 
{
        //uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
        uint64_t word = 0x3f4;
        uint64_t val = 8;
        uint64_t hello = Bitpack_newu(word, 6, 2, val);
        assert(Bitpack_getu(Bitpack_newu(word, 6, 2, val), 6, 9) == Bitpack_getu(0x3f4, 6, 9));
        assert(Bitpack_getu(Bitpack_newu(word, 6, 2, val), 6, 2) == val);
        fprintf(stderr, "num: %ld\n", hello);
}

/********** Bitpacknews ********
 *
 * Test Bitpack_news
 *
 * Parameters:
 *      none
 * Return: 
 *      none
 * Expects
 *     none
 * Notes:
 *      none
 ************************/
void Bitpacknews() 
{
        uint64_t word = 0x3f4;
        int64_t val = -1;
        int64_t hello = Bitpack_news(word, 6, 2, val);
        int64_t a = Bitpack_gets(Bitpack_news(word, 6, 2, val), 6, 9);
        int64_t b = Bitpack_gets(0x3f4, 6, 9);
        int64_t c = Bitpack_gets(Bitpack_news(word, 6, 2, val), 6, 2);
        fprintf(stderr, "num: %ld\n", hello);
        fprintf(stderr, "first val = %ld, and secound val = %ld\n", a, b);
        fprintf(stderr, "val is: %ld\n", c);
        
        assert(Bitpack_gets(Bitpack_news(word, 6, 2, val), 6, 9) == Bitpack_gets(0x3f4, 6, 9));
        
        assert(Bitpack_gets(Bitpack_news(word, 6, 2, val), 6, 2) == val);
}

/********** main ********
 *
 * run testing functions
 *
* Parameters:
 *      int argc: number of input commands 
 *      char *argv[]: input commands
 *
 * Return: int
 *
 * Expects
 *     none
 * Notes:
 *      none
 ************************/
int main(int argc, char *argv[])
{
        (void) argc;
        (void) argv;
        Bitpack_fits_u();
        Bitpack_fits_s();
        Bitpackgetu();
        Bitpackgets();
        Bitpacknewu();
        Bitpacknews(); 
}
