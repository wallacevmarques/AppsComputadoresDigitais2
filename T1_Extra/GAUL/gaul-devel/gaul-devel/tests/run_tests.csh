#!/bin/csh -f
#
#######################################################################
# gaul/tests/run_tests.csh
#######################################################################
#
# GAUL - Genetic Algorithm Utility library.
# Copyright ©2000-2002, Stewart Adcock <stewart@linux-domain.com>
#
# The latest version of this program should be available at:
# http://www.stewart-adcock.co.uk/
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.  Alternatively, if your project
# is incompatible with the GPL, I will probably agree to requests
# for permission to use the terms of any other license.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY WHATSOEVER.
#
# A full copy of the GNU General Public License should be in the file
# "COPYING" provided with this distribution; if not, see:
# http://www.gnu.org/
#
#######################################################################
#
# Synopsis:	Run the series of GAUL tests.
#
# Usage:	After compilation, adjust the specified paths, then
#		type either:
#			./run_tests.csh
#		or (on systems with PBS):
#			qsub ./run_tests.csh
#
# This is PBS stuff:
#
#PBS -N TestingGaul
#PBS -l walltime=24:0:0
##PBS -M ${EMAIL} -m ea
##PBS -j oe -o desktop:/home/stewart/Code_development/gaul-devel/tests
#

#
# Adjust this path:
#
setenv GAUL_TEST_DIR /net/home/adcock/Code_development/gaul-devel/tests/

#
# Uncomment this to unsquash the core dumps:
#
#limit coredumpsize 0

#
# Run tests.
#
setenv unique $$

setenv WKDIR /tmp/gaul-tests/${unique}
mkdir -p $WKDIR >& /dev/null
cd $WKDIR

echo ">>> Test support code."
echo ">> Running tests/test_random."
echo "Not implemented"

echo ">> Running tests/test_memory."
echo "Not implemented"

echo ">> Running tests/test_mpi."
echo "Not implemented"

echo ">> Running tests/test_lists."
echo "Not implemented"

echo ">> Running tests/test_trees."
echo "Not implemented"

echo ">> Running tests/test_tables."
echo "Not implemented"

echo ">>> Test GAUL code."
echo ">> Running examples/struggle."
echo "   - tests evolution with character based chromosomes.
${GAUL_TEST_DIR}/../examples/struggle > struggle.log
# The lowest, and final, score should be 751.50

echo ">>> All tests completed."
