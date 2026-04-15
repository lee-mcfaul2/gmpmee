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
#include "gmp.h"

void
gmpmee_array_out_str(FILE *f, int base, mpz_t *op, size_t len, char *sep) {
  int i;

  for (i = 0; i < len; i++)
    {
      mpz_out_str(f, base, op[i]);
      fprintf(f, "%c", *sep);
    }
}
