
# Copyright 2008 Douglas Wikström

# This file is part of GMP Modular Exponentiation Extension (GMPMEE).

# GMPMEE is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# GMPMEE is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
# License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with GMPMEE.  If not, see
# <http://www.gnu.org/licenses/>.

.PHONY: clean cleandoc

ACLOCAL_AMFLAGS=-I m4

if BITS64
   AM_CFLAGS = -fPIC -m64 # 64-bit machine
else
   AM_CFLAGS = -fPIC      # 32-bit machine
endif

AM_LDFLAGS = -lgmp

# Actual target.
lib_LTLIBRARIES = libgmpmee.la
libgmpmee_la_SOURCES = spowm.c spowm_clear.c spowm_precomp.c spowm_block.c spowm_init.c spowm_table.c spowm_block_batch.c spowm_init_precomp.c spowm_naive.c array_alloc.c array_clear_dealloc.c array_urandomb.c array_alloc_init.c array_out_str.c fpowm.c fpowm_clear.c fpowm_init.c fpowm_precomp.c fpowm_init_precomp.c millerrabin_init.c millerrabin_next_cand.c millerrabin_clear.c millerrabin_trial.c millerrabin_once.c millerrabin_reps_rs.c millerrabin_rs.c millerrabin_next_rs.c millerrabin_safe_init.c millerrabin_safe_next_cand.c millerrabin_safe_clear.c millerrabin_safe_trial.c millerrabin_safe_reps_rs.c millerrabin_safe_rs.c millerrabin_safe_next_rs.c probab_prime_p_next.c probab_safe_prime_p.c probab_safe_prime_p_next.c

# Conditional compilation of redc-representation of moduli.
if REDC
libgmpmee_la_SOURCES += backwards_mpn_redc_1.c redc_clear.c redc_init_set.c redc_encode.c redc_decode.c redc_mod.c
endif

include_HEADERS = gmpmee.h

# Testing and timing.
noinst_PROGRAMS = simexp_test simexp_time fixexp_test fixexp_time millerrabin_test

simexp_test_SOURCES = simexp_test.c
simexp_test_LDADD = libgmpmee.la

simexp_time_SOURCES = simexp_time.c
simexp_time_LDADD = libgmpmee.la

fixexp_test_SOURCES = fixexp_test.c
fixexp_test_LDADD = libgmpmee.la

fixexp_time_SOURCES = fixexp_time.c
fixexp_time_LDADD = libgmpmee.la

millerrabin_test_SOURCES = millerrabin_test.c
millerrabin_test_LDADD = libgmpmee.la


dist_noinst_DATA = .doc doxygen.cfg trialdiv_32.c trialdiv_64.c trialdiv_safe_32.c trialdiv_safe_64.c .version.m4


doc: .doc.stamp
.doc.stamp:
	mkdir -p doc/
	cp -R .doc/html/* doc/
	doxygen doxygen.cfg
	rm -rf doc/api
	mv doc/html doc/api
	@touch .doc.stamp

cleandoc:
	rm -f .doc.stamp
	rm -rf doc

clean-local: cleandoc
	rm -f *~
