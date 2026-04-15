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

void
gmpmee_millerrabin_safe_clear(gmpmee_millerrabin_safe_state state)
{
  gmpmee_millerrabin_clear(state->nstate);
  gmpmee_millerrabin_clear(state->mstate);
}
