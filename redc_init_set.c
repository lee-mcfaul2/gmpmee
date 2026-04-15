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
gmpmee_redc_init_set (gmpmee_redc_t r, mpz_srcptr m)
{
  mpz_ptr rz;
  mp_ptr mp;
  mp_limb_t m0;
  mp_limb_t binv;

  mp = PTR(m);
  m0 = mp[0];
  if (m0 % 2 == 0)
    DIVIDE_BY_ZERO;
  binvert_limb (binv, m0);
  BINV(r) = -binv;

  rz = MPZ(r);
  mpz_init_set (rz, m);
}
