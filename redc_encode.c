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

static void
redcify (mp_ptr rp, mp_srcptr up, mp_size_t un, mp_srcptr mp, mp_size_t n)
{
  mp_ptr tp, qp;
  TMP_DECL;
  TMP_MARK;

  tp = TMP_ALLOC_LIMBS (un + n);
  qp = TMP_ALLOC_LIMBS (un + 1);	/* FIXME: Put at tp+? */

  MPN_ZERO (tp, n);
  MPN_COPY (tp + n, up, un);
  mpn_tdiv_qr (qp, rp, 0L, tp, un + n, mp, n);
  TMP_FREE;
}

/* Compute r = B^n mod u, n comes from MPZ(mrt).  */
void
gmpmee_redc_encode (mpz_ptr r, mpz_srcptr u, gmpmee_redc_t mrt)
{
  mp_size_t un, n;
  mp_ptr rp, up, mp;
  mpz_srcptr m;

  un = SIZ(u);
  if (un < 0)
    DIVIDE_BY_ZERO;		/* FIXME */

  m = MPZ(mrt);
  n = ABSIZ(m);

  MPZ_REALLOC (r, n);
  rp = PTR(r);
  up = PTR(u);
  mp = PTR(m);

  redcify (rp, up, un, mp, n);
  SIZ(r) = n;
}
