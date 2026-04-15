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

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

#include <gmp.h>
#include "gmpmee.h"

char *command_name;

void usage() {
  printf("Usage %s: (-n|-f) <exponentiations> <modulus bit len> <actual exponent bit len> [<exponent bit len> <block width>]\n",
	 command_name);
  exit(0);
}

int main(int argc, char *argv[]) {
  int i;
  int exponentiations;
  int modulus_bitlen;
  int exponent_bitlen;
  int block_width;
  int actual_exponent_bitlen;

  gmp_randstate_t state;

  gmpmee_fpowm_tab table;

  mpz_t base;
  mpz_t modulus;
  mpz_t res;
  mpz_t *exponents;

  clock_t c0 = 0;
  clock_t c1 = 0;

  mpz_init(base);
  mpz_init(modulus);
  mpz_init(res);

  command_name = argv[0];

  if (!(argc == 7 || (argc == 5 && strcmp(argv[1], "-n") == 0))) {
    usage();
  }

  if (sscanf(argv[2], "%d", &exponentiations) != 1) {
    usage();
  }
  if (sscanf(argv[3], "%d", &modulus_bitlen) != 1) {
    usage();
  }
  if (sscanf(argv[4], "%d", &actual_exponent_bitlen) != 1) {
    usage();
  }
  if (argc > 5) {
    if (sscanf(argv[5], "%d", &exponent_bitlen) != 1) {
      usage();
    }
    if (sscanf(argv[6], "%d", &block_width) != 1) {
      usage();
    }
  }

  /* Initialize random state. */
  gmp_randinit_default(state);

  mpz_urandomb(modulus, state, modulus_bitlen);
  if (mpz_cmp_ui(modulus, 0) == 0) {
    mpz_set_ui(modulus, 1);
  }
  mpz_urandomb(base, state, modulus_bitlen);
  mpz_mod(base, base, modulus);
  if (mpz_cmp_ui(base, 0) == 0) {
    mpz_set_ui(base, 2);
  }

  exponents = gmpmee_array_alloc_init(exponentiations);
  gmpmee_array_urandomb(exponents, exponentiations, state,
		     actual_exponent_bitlen);

  if (strcmp(argv[1], "-f") == 0) {
    gmpmee_fpowm_init_precomp(table, base, modulus, block_width,
			   exponent_bitlen);
  }

  c0 = clock();
  if (strcmp(argv[1], "-f") == 0)
    {
      for (i = 0; i < exponentiations; i++) {
	gmpmee_fpowm(res, table, exponents[i]);
      }
    }
  else if (strcmp(argv[1], "-n") == 0)
    {
      for (i = 0; i < exponentiations; i++) {
	mpz_powm(res, base, exponents[i], modulus);
      }
    }
  else
    {
      usage();
    }
  c1 = clock();


  /* Print cpu time. */
  printf("%.2f\n", ((float)(c1 - c0)) / CLOCKS_PER_SEC);

  return 0;
}
