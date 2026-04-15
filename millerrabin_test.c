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

void usage(char *command_name)
{
  printf("Usage %s: -testp|-testsp|-testnp|-testnsp|-gmpp|-p|-sp|-np|-nsp <bitsize> <iters> <reps>\n", command_name);
  exit(0);
}

int main(int argc, char *argv[])
{
  int res;
  int mode = 0;
  int i;
  int bitsize;
  int iters;
  int reps;
  mpz_t n;
  mpz_t rop;
  mpz_t gmprop;
  gmp_randstate_t rstate;

  mpz_init(n);
  mpz_init(rop);
  mpz_init(gmprop);
  gmp_randinit_default(rstate);

  if (argc != 5)
    {
      usage(argv[0]);
    }

  i = 1;
  if (argc == 5) {
    if (strcmp(argv[i], "-testp") == 0) {
      mode = 0;
    } else if (strcmp(argv[i], "-testsp") == 0) {
      mode = 1;
    } else if (strcmp(argv[i], "-testnp") == 0) {
      mode = 2;
    } else if (strcmp(argv[i], "-testnsp") == 0) {
      mode = 3;
    } else if (strcmp(argv[i], "-gmpp") == 0) {
      mode = 4;
    } else if (strcmp(argv[i], "-p") == 0) {
      mode = 5;
    } else if (strcmp(argv[i], "-sp") == 0) {
      mode = 6;
    } else if (strcmp(argv[i], "-np") == 0) {
      mode = 7;
    } else if (strcmp(argv[i], "-nsp") == 0) {
      mode = 8;
    } else {
      usage(argv[0]);
    }
    i++;
  }

  if (sscanf(argv[i++], "%d", &bitsize) != 1) {
    usage(argv[0]);
  }
  if (sscanf(argv[i++], "%d", &iters) != 1) {
    usage(argv[0]);
  }
  if (sscanf(argv[i++], "%d", &reps) != 1) {
    usage(argv[0]);
  }

  fprintf(stdout, "%s: bitsize = %d, iters = %d, reps = %d\n",
	  argv[1]+1, bitsize, iters, reps);

  mpz_urandomb(n, rstate, bitsize);

  for (i = 0; i < iters; i++)
    {
      mpz_add_ui(n, n, 1L);

      if (mode == 0)
	{
	  res = mpz_probab_prime_p(n, reps) ? 1 : 0;
	  if (res != gmpmee_millerrabin_rs(rstate, n, reps)) {
	    fprintf(stderr, "ERRORR");
	    fprintf(stderr, "n = ");
	    mpz_out_str (stderr, 16, n);
	    fprintf(stderr, "\n");
	    exit(0);
	  }
	}
      else if (mode == 1)
	{
	  res = mpz_probab_safe_prime_p(n, reps) ? 1 : 0;
	  if (res != gmpmee_millerrabin_safe_rs(rstate, n, reps)) {
	    fprintf(stderr, "ERRORR");
	    fprintf(stderr, "n = ");
	    mpz_out_str (stderr, 16, n);
	    fprintf(stderr, "\n");
	    exit(0);
	  }
	}
      else if (mode == 2)
	{
	  mpz_probab_prime_p_next(gmprop, n, reps);
	  gmpmee_millerrabin_next_rs(rop, rstate, n, reps);
	  if (mpz_cmp(gmprop, rop) != 0) {
	    fprintf(stderr, "ERRORR\n");
	    fprintf(stderr, "gmprop = ");
	    mpz_out_str (stderr, 16, gmprop);
	    fprintf(stderr, "\nrop = ");
	    mpz_out_str (stderr, 16, rop);
	    fprintf(stderr, "\n");
	    exit(0);
	  }
	}
      else if (mode == 3)
	{

	  mpz_probab_safe_prime_p_next(gmprop, n, reps);
	  gmpmee_millerrabin_safe_next_rs(rop, rstate, n, reps);
	  if (mpz_cmp(gmprop, rop) != 0) {
	    fprintf(stderr, "ERRORR\n");
	    fprintf(stderr, "gmprop = ");
	    mpz_out_str (stderr, 16, gmprop);
	    fprintf(stderr, "\nrop = ");
	    mpz_out_str (stderr, 16, rop);
	    fprintf(stderr, "\n");
	    exit(0);
	  }
	}
      else if (mode == 4)
	{
	  res = mpz_probab_prime_p(n, reps);
	}
      else if (mode == 5)
	{
	  res = gmpmee_millerrabin_rs(rstate, n, reps);
	}
      else if (mode == 6)
	{
	  res = gmpmee_millerrabin_safe_rs(rstate, n, reps);
	}
      else if (mode == 7)
	{
	  gmpmee_millerrabin_next_rs(rop, rstate, n, reps);
	}
      else if (mode == 8)
	{
	  gmpmee_millerrabin_safe_next_rs(rop, rstate, n, reps);
	}
    }

  mpz_clear(gmprop);
  mpz_clear(rop);
  gmp_randclear(rstate);
}

