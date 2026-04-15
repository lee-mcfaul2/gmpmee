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
gmpmee_millerrabin_safe_rs(gmp_randstate_t rstate, mpz_t n, int reps)
{

  int res;
  gmpmee_millerrabin_safe_state state;

  if (gmpmee_millerrabin_safe_trial(n) == 0)
    {
      return 0;
    }

  gmpmee_millerrabin_safe_init(state, n);
  res = gmpmee_millerrabin_safe_reps_rs(rstate, state, reps);
  gmpmee_millerrabin_safe_clear(state);

  return res;
}
