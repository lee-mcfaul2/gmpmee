/*

Copyright 2008 2009 Torbjörn Granlund, Douglas Wikström

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

Based on code from GNU MP Library contributed by John Amanatides and
Torbjörn Granlund.

*/

#include "gmp.h"
#include "gmpmee.h"

int
gmpmee_millerrabin_reps_rs(gmp_randstate_t rstate,
			   gmpmee_millerrabin_state state,
			   int reps)
{
  int i;
  int res = 1;
  mpz_t base;
  mpz_t n_minus_3;

  mpz_init(n_minus_3);
  mpz_sub_ui(n_minus_3, state->n, 3);
  mpz_init(base);

  for (i = 0; i < reps; i++) {

    /* Random base in [2,n-1] */
    mpz_urandomm(base, rstate, n_minus_3);
    mpz_add_ui(base, base, 2);

    if (gmpmee_millerrabin_once(state, base) == 0)
      {
	res = 0;
	break;
      }
  }

  mpz_clear(base);
  mpz_clear(n_minus_3);

  return res;
}
