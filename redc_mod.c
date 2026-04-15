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
gmpmee_redc_mod (mpz_ptr r, mpz_srcptr u, gmpmee_redc_t mrt)
{
  mp_ptr rp, up, mp, tp;
  mp_limb_t mbinv;
  mp_size_t un, n;
  mpz_srcptr m;
  TMP_DECL;
  TMP_MARK;

  up = PTR(u);
  un = SIZ(u);
  if (un < 0)
    DIVIDE_BY_ZERO;

  m = MPZ(mrt);
  n = SIZ(m);

  tp = TMP_ALLOC_LIMBS (2 * n);
  MPN_COPY (tp, up, un);
  MPN_ZERO (tp + un, 2 * n - un);

  MPZ_REALLOC (r, n);
  rp = PTR(r);
  mp = PTR(m);

  mbinv = BINV(mrt);

  gmpmee_mpn_redc_1(rp, tp, mp, n, mbinv);
  SIZ(r) = n;

  TMP_FREE;
}
