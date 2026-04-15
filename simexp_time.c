/*

Copyright 2008 2009 Douglas Wikström

This file is part of GMP Modular Exponentiation Extension (GMPMEE).

GMPMEE is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

GMPMEE is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public
License along with GMPMEE.  If not, see
<http://www.gnu.org/licenses/>.

*/

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include <gmp.h>
#include "gmpmee.h"

#define NAIVE 0
#define PRECOMP 1
#define SIMEXP 2
#define PRECOMP_AND_SIMEXP 3

char *command_name;

void usage() {
  printf("Usage %s: (-n|-p|-s|-ps) <len> <modulus bit len> <exponents bit len> [<block width>]\n",
	 command_name);
  printf("%4s %s\n", "-n", "Naive implementation.");
  printf("%4s %s\n", "-p", "Precomputation for simultaneous exponentiation.");
  printf("%4s %s\n", "-s",
	 "Simultaneous exponentiation (not counting precomputation)");
  printf("%4s %s\n", "-ps",
	 "Simultaneous implementation including precomputation.");
  exit(0);
}

int main(int argc, char *argv[]) {

  int i;
  int operation = 0;

  int len;
  int modulus_bitlen;
  int exponents_bitlen;
  int block_width;

  gmp_randstate_t state;
  mpz_t modulus;
  mpz_t *bases;
  mpz_t *exponents;
  mpz_t res;
  gmpmee_spowm_tab table;

  clock_t c0 = 0;
  clock_t c1 = 0;

  command_name = argv[0];

  if (argc < 5)
    {
      usage();
    }

  /* Parse command line parameters. */
  if (strcmp(argv[1], "-n") == 0)
    {
      operation = NAIVE;
    }
  else if (strcmp(argv[1], "-p") == 0)
    {
      operation = PRECOMP;
    }
  else if (strcmp(argv[1], "-s") == 0)
    {
      operation = SIMEXP;
    }
  else if (strcmp(argv[1], "-ps") == 0)
    {
      operation = PRECOMP_AND_SIMEXP;
    }
  else
    {
      usage();
    }

  if (sscanf(argv[2], "%d", &len) != 1)
    {
      usage();
    }
  if (sscanf(argv[3], "%d", &modulus_bitlen) != 1)
    {
      usage();
    }
  if (sscanf(argv[4], "%d", &exponents_bitlen) != 1)
    {
      usage();
    }
  if (argc == 6)
    {
      if (sscanf(argv[5], "%d", &block_width) != 1)
	{
	  usage();
	}
    }
  else
    {
      block_width = 5;
    }

  /* Initialize random state. */
  gmp_randinit_default(state);
  gmp_randseed_ui(state, 0);

  /* Generate modulus. */
  mpz_init(modulus);
  mpz_urandomb(modulus, state, modulus_bitlen);

#ifdef REDC

  /* Make sure it is odd. */
  mpz_setbit(modulus, 0);

#endif

  /* Generate bases. */
  bases = gmpmee_array_alloc_init(len);
  gmpmee_array_urandomb(bases, len, state, modulus_bitlen);
  for (i = 0; i < len; i++)
    {
      mpz_mod(bases[i], bases[i], modulus);
    }

  /* Generate exponents. */
  exponents = gmpmee_array_alloc_init(len);
  gmpmee_array_urandomb(exponents, len, state, exponents_bitlen);

  mpz_init(res);

  /* Time execution. */

  switch (operation)
    {
    case NAIVE:
      c0 = clock();
      gmpmee_spowm_naive(res, bases, exponents, len, modulus);
      c1 = clock();
      break;
    case PRECOMP:
      c0 = clock();
      gmpmee_spowm_init_precomp(table, bases, len, modulus, block_width);
      c1 = clock();
      gmpmee_spowm_clear(table);
      break;
    case SIMEXP:
      gmpmee_spowm_init_precomp(table, bases, len, modulus, block_width);
      c0 = clock();
      gmpmee_spowm_table(res, table, exponents);
      c1 = clock();
      gmpmee_spowm_clear(table);
    case PRECOMP_AND_SIMEXP:
      c0 = clock();
      gmpmee_spowm_block(res, bases, exponents, len, modulus, block_width);
      c1 = clock();
      break;
    default:
      usage();
    }

  /* Print cpu time. */
  printf("%.2f\n", ((float)(c1 - c0)) / CLOCKS_PER_SEC);

  return 0;
}
