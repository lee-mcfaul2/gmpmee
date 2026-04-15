/*

Copyright 2008 2009 Torbjörn Granlund

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

#include "gmp.h"
#include "gmp-impl.h"
#include "gmpmee_redc.h"

void
gmpmee_redc_decode (mpz_ptr r, mpz_srcptr u, gmpmee_redc_t mrt)
{
  mp_ptr rp, mp;
  mp_size_t n;
  mpz_srcptr m;

  gmpmee_redc_mod (r, u, mrt);

  m = MPZ(mrt);
  n = SIZ(m);

  rp = PTR(r);
  mp = PTR(m);

  if (mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);

  MPN_NORMALIZE (rp, n);
  SIZ(r) = n;
}
