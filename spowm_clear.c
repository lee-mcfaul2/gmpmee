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

*/

#include <stdlib.h>
#include "gmp.h"
#include "gmpmee.h"

void
gmpmee_spowm_clear(gmpmee_spowm_tab table)
{
  int i, j;
  size_t tabs_len = table->tabs_len;
  size_t block_width = table->block_width;
  size_t tab_len = 1 << block_width;
  mpz_t *t;

  for (i = 0; i < tabs_len; i++)
    {

      /* Last block may have smaller width. */
      if (i == tabs_len - 1)
	{
	  block_width = table->len - (tabs_len - 1) * block_width;
	  tab_len = 1 << block_width;
	}

      /* Deallocate all integers in table. */
      t = table->tabs[i];
      for (j = 0; j < tab_len; j++)
	{
	  mpz_clear(t[j]);
	}

      /* Deallocate table. */
      free(t);
    }

  /* Deallocate table of tables. */
  free(table->tabs);

  /* Clear representation of modulus. */
#ifdef REDC
  if (mpz_tstbit(table->modulus, 0))
    {
      gmpmee_redc_clear(table->mrt);
    }
#endif
  mpz_clear(table->modulus);

}
