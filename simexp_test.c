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

#include <gmp.h>
#include "gmpmee.h"

char *command_name;

void usage() {
  printf("Usage %s: <len> <modulus bit len> <exponents bit len> " \
	 "[<block width>] [<batch len>]\n",
	 command_name);
  exit(0);
}

int main(int argc, char *argv[]) {
  int j, i;
  int len;
  int modulus_bitlen;
  int exponents_bitlen;
  int block_width;
  int batch_len;

  gmp_randstate_t state;
  mpz_t modulus;
  mpz_t *bases;
  mpz_t *exponents;

  mpz_t naive_res;
  mpz_t sim_res;

  command_name = argv[0];

  if (argc < 4) {
    usage();
  }

  if (sscanf(argv[1], "%d", &len) != 1) {
    usage();
  }
  if (sscanf(argv[2], "%d", &modulus_bitlen) != 1) {
    usage();
  }
  if (sscanf(argv[3], "%d", &exponents_bitlen) != 1) {
    usage();
  }
  if (argc >= 5) {
    if (sscanf(argv[4], "%d", &block_width) != 1) {
      usage();
    }
  } else {
    block_width = 1;
  }

  if (argc == 6) {
    if (sscanf(argv[5], "%d", &batch_len) != 1) {
      usage();
    }
  } else {
    batch_len = len;
  }


  /* Initialize random state. */
  gmp_randinit_default(state);

  mpz_init(modulus);
  mpz_init(naive_res);
  mpz_init(sim_res);
  bases = gmpmee_array_alloc_init(len);
  exponents = gmpmee_array_alloc_init(len);


  for (j = 0; j < 2; j++)
    {

      /* Generate modulus. */
      mpz_urandomb(modulus, state, modulus_bitlen);

      /* Test both even and odd moduli. */
      if (j == 0) {
	mpz_clrbit(modulus, 0);
      } else {
	mpz_setbit(modulus, 0);
      }

      if (mpz_cmp_ui(modulus, 0) == 0) {
	mpz_setbit(modulus, 0);
      }

      /* Generate bases. */
      gmpmee_array_urandomb(bases, len, state, modulus_bitlen);
      for (i = 0; i < len; i++)
	{
	  mpz_mod(bases[i], bases[i], modulus);
	}

      /* Generate exponents. */
      gmpmee_array_urandomb(exponents, len, state, exponents_bitlen);

      gmpmee_spowm_naive(naive_res, bases, exponents, len, modulus);

      gmpmee_spowm_block_batch(sim_res, bases, exponents, len, modulus,
			       block_width, batch_len);

      if (mpz_cmp(sim_res, naive_res) != 0)
	{
	  printf("Error!");
	}
    }
  return 0;
}
