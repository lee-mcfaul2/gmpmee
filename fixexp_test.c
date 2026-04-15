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
  printf("Usage %s: <max modulus bit len> <max exponent bit len> <max block width>\n",
	 command_name);
  exit(0);
}


void perform_test(gmp_randstate_t state,
		  int max_modulus_bitlen,
		  int max_exponent_bitlen,
		  int max_block_width) {
  int modulus_bitlen;
  int exponent_bitlen;
  int block_width;

  gmpmee_fpowm_tab table;

  mpz_t base;
  mpz_t modulus;
  mpz_t exponent;
  mpz_t fix_res;
  mpz_t res;

  mpz_init(base);
  mpz_init(modulus);
  mpz_init(exponent);
  mpz_init(fix_res);
  mpz_init(res);

  for (modulus_bitlen = 2;
       modulus_bitlen <= max_modulus_bitlen;
       modulus_bitlen++)
    {

      mpz_urandomb(modulus, state, modulus_bitlen);
      if (mpz_cmp_ui(modulus, 0) == 0) {
	mpz_set_ui(modulus, 1);
      }
      mpz_urandomb(base, state, modulus_bitlen);
      mpz_mod(base, base, modulus);
      if (mpz_cmp_ui(base, 0) == 0) {
	mpz_set_ui(base, 2);
      }

      for (block_width = 1;
	   block_width <= max_block_width;
	   block_width++)
	{

	  gmpmee_fpowm_init_precomp(table, base, modulus, block_width,
				 max_exponent_bitlen / 2);

	  for (exponent_bitlen = 1;
	       exponent_bitlen <= max_exponent_bitlen;
	       exponent_bitlen++)
	    {

	      mpz_urandomb(exponent, state, max_exponent_bitlen / 2);

	      gmpmee_fpowm(fix_res, table, exponent);
	      mpz_powm(res, base, exponent, modulus);
	      if (mpz_cmp(fix_res, res) != 0)
		{
		  fprintf(stderr, "Error!");
		}

	    }

	  gmpmee_fpowm_clear(table);
	}
    }
}

int main(int argc, char *argv[]) {
  int max_modulus_bitlen;
  int max_exponent_bitlen;
  int max_block_width;

  gmp_randstate_t state;

  command_name = argv[0];

  if (argc != 4) {
    usage();
  }

  if (sscanf(argv[1], "%d", &max_modulus_bitlen) != 1) {
    usage();
  }
  if (sscanf(argv[2], "%d", &max_exponent_bitlen) != 1) {
    usage();
  }
  if (sscanf(argv[3], "%d", &max_block_width) != 1) {
    usage();
  }

  /* Initialize random state. */
  gmp_randinit_default(state);

  perform_test(state, max_modulus_bitlen, max_exponent_bitlen, max_block_width);

  return 0;
}
