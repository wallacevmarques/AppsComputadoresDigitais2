/**********************************************************************
  ga_mutate.c
 **********************************************************************

  ga_mutate - Genetic algorithm mutation operators.
  Copyright ©2000-2003, Stewart Adcock <stewart@linux-domain.com>
  All rights reserved.

  The latest version of this program should be available at:
  http://gaul.sourceforge.net/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.  Alternatively, if your project
  is incompatible with the GPL, I will probably agree to requests
  for permission to use the terms of any other license.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY WHATSOEVER.

  A full copy of the GNU General Public License should be in the file
  "COPYING" provided with this distribution; if not, see:
  http://www.gnu.org/

 **********************************************************************

  Synopsis:     Routines for performing GA mutation operations.

		These functions should duplicate user data where
		appropriate.

 **********************************************************************/

#include "gaul/ga_core.h"

/**********************************************************************
  ga_mutate_integer_singlepoint_drift()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is cycled.
  parameters:
  return:
  last updated: 01/09/00
 **********************************************************************/

void ga_mutate_integer_singlepoint_drift( population *pop,
                                          entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  int		dir=random_boolean()?-1:1;	/* The direction of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/*
 * Copy unchanged data.
 */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(int));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

/*
 * Mutate by tweaking a single nucleotide.
 */
  ((int *)son->chromosome[chromo])[point] += dir;

  if (((int *)son->chromosome[chromo])[point]==RAND_MAX) ((int *)son->chromosome[chromo])[point]=0;
  if (((int *)son->chromosome[chromo])[point]==-1) ((int *)son->chromosome[chromo])[point]=RAND_MAX-1;

  return;
  }


/**********************************************************************
  ga_mutate_integer_singlepoint_randomize()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is randomized.
  parameters:
  return:
  last updated: 01/09/00
 **********************************************************************/

void ga_mutate_integer_singlepoint_randomize( population *pop,
                                              entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/* Copy unchanging data. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(int));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

  ((int *)son->chromosome[chromo])[point] = (int) random_int(RAND_MAX);

  return;
  }


/**********************************************************************
  ga_mutate_integer_multipoint()
  synopsis:	Cause a number of mutation events.  This is equivalent
		to the more common 'bit-drift' mutation.
  parameters:
  return:
  last updated: 21/07/00
 **********************************************************************/

void ga_mutate_integer_multipoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  int		dir=random_boolean()?-1:1;	/* The direction of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Copy chromosomes of parent to offspring. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(int));
    }

/*
 * Mutate by tweaking nucleotides.
 */
  for (chromo=0; chromo<pop->num_chromosomes; chromo++)
    {
    for (point=0; point<pop->len_chromosomes; point++)
      {
      if (random_boolean_prob(GA_MULTI_BIT_CHANCE))
        {
        ((int *)son->chromosome[chromo])[point] += dir;

        if (((int *)son->chromosome[chromo])[point]==RAND_MAX)
          ((int *)son->chromosome[chromo])[point]=0;
        if (((int *)son->chromosome[chromo])[point]==-1)
          ((int *)son->chromosome[chromo])[point]=RAND_MAX-1;
        }
      }
    }

  return;
  }


/**********************************************************************
  ga_mutate_integer_allpoint()
  synopsis:	Cause a number of mutation events.  Each allele has
		equal probability of being incremented, decremented, or
		remaining the same.
  parameters:
  return:
  last updated: 28 May 2002
 **********************************************************************/

void ga_mutate_integer_allpoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Copy chromosomes of parent to offspring. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(int));
    }

/*
 * Mutate by incrementing or decrementing alleles.
 */
  for (chromo=0; chromo<pop->num_chromosomes; chromo++)
    {
    for (point=0; point<pop->len_chromosomes; point++)
      {
      switch (random_int(3))
        {
        case (1):
          (((int *)son->chromosome[chromo])[point])++;

          if (((int *)son->chromosome[chromo])[point]==RAND_MAX)
            ((int *)son->chromosome[chromo])[point]=0;

          break;

        case (2):
          (((int *)son->chromosome[chromo])[point])--;

          if (((int *)son->chromosome[chromo])[point]==-1)
            ((int *)son->chromosome[chromo])[point]=RAND_MAX-1;

          break;

        default:
          /* Do nothing. */
          break;
        }
      }
    }

  return;
  }


/**********************************************************************
  ga_mutate_boolean_singlepoint()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is inverted.
  parameters:
  return:
  last updated: 31/05/01
 **********************************************************************/

void ga_mutate_boolean_singlepoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/* Copy unchanging data. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(boolean));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

  ((boolean *)son->chromosome[chromo])[point] = !((boolean *)son->chromosome[chromo])[point];

  return;
  }


/**********************************************************************
  ga_mutate_boolean_multipoint()
  synopsis:	Cause a number of mutation events.
  parameters:
  return:
  last updated: 31/05/01
 **********************************************************************/

void ga_mutate_boolean_multipoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Copy chromosomes of parent to offspring. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(boolean));
    }

/*
 * Mutate by flipping random bits.
 */
  for (chromo=0; chromo<pop->num_chromosomes; chromo++)
    {
    for (point=0; point<pop->len_chromosomes; point++)
      {
      if (random_boolean_prob(GA_MULTI_BIT_CHANCE))
        {
        ((boolean *)son->chromosome[chromo])[point] = !((boolean *)son->chromosome[chromo])[point];
        }
      }
    }

  return;
  }


/**********************************************************************
  ga_mutate_char_singlepoint_drift()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is cycled.
  parameters:
  return:
  last updated: 16/06/01
 **********************************************************************/

void ga_mutate_char_singlepoint_drift( population *pop,
                                       entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  int		dir=random_boolean()?-1:1;	/* The direction of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/*
 * Copy unchanged data.
 */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(char));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

/*
 * Mutate by tweaking a single nucleotide.
 */
  ((char *)son->chromosome[chromo])[point] += dir;

/* Don't need these because char's **should** wrap safely.
  if (((char *)son->chromosome[chromo])[point]>CHAR_MAX)
    ((char *)son->chromosome[chromo])[point]=CHAR_MIN;
  if (((char *)son->chromosome[chromo])[point]<CHAR_MIN)
    ((char *)son->chromosome[chromo])[point]=CHAR_MAX;
*/

  return;
  }


/**********************************************************************
  ga_mutate_char_allpoint()
  synopsis:	Cause a number of mutation events.  Each allele has
		equal probability of being incremented, decremented, or
		remaining the same.
  parameters:
  return:
  last updated: 03 Jun 2002
 **********************************************************************/

void ga_mutate_char_allpoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Copy chromosomes of parent to offspring. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(int));
    }

/*
 * Mutate by incrementing or decrementing alleles.
 */
  for (chromo=0; chromo<pop->num_chromosomes; chromo++)
    {
    for (point=0; point<pop->len_chromosomes; point++)
      {
      switch (random_int(3))
        {
        case (1):
          (((char *)son->chromosome[chromo])[point])++;

          break;

        case (2):
          (((char *)son->chromosome[chromo])[point])--;

          break;

        default:
          /* Do nothing. */
          break;
        }
      }
    }

  return;
  }



/**********************************************************************
  ga_mutate_char_singlepoint_randomize()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is randomized.
  parameters:
  return:
  last updated: 16/06/01
 **********************************************************************/

void ga_mutate_char_singlepoint_randomize( population *pop,
                                           entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/* Copy unchanging data. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(char));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

  ((char *)son->chromosome[chromo])[point] = (int) random_int(CHAR_MAX-CHAR_MIN)+CHAR_MIN;

  return;
  }


/**********************************************************************
  ga_mutate_char_multipoint()
  synopsis:	Cause a number of mutation events.  This is equivalent
		to the more common 'bit-drift' mutation.
  parameters:
  return:
  last updated: 16/06/01
 **********************************************************************/

void ga_mutate_char_multipoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  int		dir=random_boolean()?-1:1;	/* The direction of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Copy chromosomes of parent to offspring. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(char));
    }

/*
 * Mutate by tweaking nucleotides.
 */
  for (chromo=0; chromo<pop->num_chromosomes; chromo++)
    {
    for (point=0; point<pop->len_chromosomes; point++)
      {
      if (random_boolean_prob(GA_MULTI_BIT_CHANCE))
        {
        ((char *)son->chromosome[chromo])[point] += dir;

/* Don't need these because char's **should** wrap safely.
        if (((char *)son->chromosome[chromo])[point]>CHAR_MAX)
          ((char *)son->chromosome[chromo])[point]=CHAR_MIN;
        if (((char *)son->chromosome[chromo])[point]<CHAR_MIN)
          ((char *)son->chromosome[chromo])[point]=CHAR_MAX;
*/
        }
      }
    }

  return;
  }


/**********************************************************************
  ga_mutate_printable_singlepoint_drift()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is cycled.
  parameters:
  return:
  last updated: 16/06/01
 **********************************************************************/

void ga_mutate_printable_singlepoint_drift( population *pop,
                                       entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  int		dir=random_boolean()?-1:1;	/* The direction of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/*
 * Copy unchanged data.
 */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(char));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

/*
 * Mutate by tweaking a single nucleotide.
 */
  ((char *)son->chromosome[chromo])[point] += dir;

  if (((char *)son->chromosome[chromo])[point]>'~')
    ((char *)son->chromosome[chromo])[point]=' ';
  if (((char *)son->chromosome[chromo])[point]<' ')
    ((char *)son->chromosome[chromo])[point]='~';

  return;
  }


/**********************************************************************
  ga_mutate_printable_singlepoint_randomize()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is randomized.
  parameters:
  return:
  last updated: 16/06/01
 **********************************************************************/

void ga_mutate_printable_singlepoint_randomize( population *pop,
                                           entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/* Copy unchanging data. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(char));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

  ((char *)son->chromosome[chromo])[point] = (int) random_int('~'-' ')+' ';

  return;
  }


/**********************************************************************
  ga_mutate_printable_multipoint()
  synopsis:	Cause a number of mutation events.  This is equivalent
		to the more common 'bit-drift' mutation.
  parameters:
  return:
  last updated: 16/06/01
 **********************************************************************/

void ga_mutate_printable_multipoint(population *pop, entity *father, entity *son)
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  int		dir=random_boolean()?-1:1;	/* The direction of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Copy chromosomes of parent to offspring. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(char));
    }

/*
 * Mutate by tweaking nucleotides.
 */
  for (chromo=0; chromo<pop->num_chromosomes; chromo++)
    {
    for (point=0; point<pop->len_chromosomes; point++)
      {
      if (random_boolean_prob(GA_MULTI_BIT_CHANCE))
        {
        ((char *)son->chromosome[chromo])[point] += dir;

        if (((char *)son->chromosome[chromo])[point]>'~')
          ((char *)son->chromosome[chromo])[point]=' ';
        if (((char *)son->chromosome[chromo])[point]<' ')
          ((char *)son->chromosome[chromo])[point]='~';
        }
      }
    }

  return;
  }


/**********************************************************************
  ga_mutate_bitstring_singlepoint()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is flipped.
  parameters:
  return:
  last updated: 30/06/01
 **********************************************************************/

void ga_mutate_bitstring_singlepoint( population *pop,
                                    entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/* Copy unchanging data. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    ga_bit_clone(son->chromosome[i], father->chromosome[i], pop->len_chromosomes);

    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

/* The singlepoint mutation. */
  ga_bit_invert(son->chromosome[chromo],point);

  return;
  }


/**********************************************************************
  ga_mutate_double_singlepoint_drift()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is adjusted.  (Unit Gaussian distribution.)
  parameters:
  return:
  last updated: 19 Apr 2002
 **********************************************************************/

void ga_mutate_double_singlepoint_drift( population *pop,
                                          entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */
  double	amount=random_unit_gaussian();	/* The amount of drift. */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/*
 * Copy unchanged data.
 */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(double));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

/*
 * Mutate by tweaking a single nucleotide.
 */
  ((double *)son->chromosome[chromo])[point] += amount;

  if (((double *)son->chromosome[chromo])[point]>DBL_MAX-1.0) ((double *)son->chromosome[chromo])[point]=DBL_MIN+1.0;
  if (((double *)son->chromosome[chromo])[point]<DBL_MIN+1.0) ((double *)son->chromosome[chromo])[point]=DBL_MAX-1.0;

  return;
  }


/**********************************************************************
  ga_mutate_double_singlepoint_randomize()
  synopsis:	Cause a single mutation event in which a single
		nucleotide is randomized.  (Unit Gaussian distribution.)
  parameters:
  return:
  last updated: 19 Apr 2002
 **********************************************************************/

void ga_mutate_double_singlepoint_randomize( population *pop,
                                              entity *father, entity *son )
  {
  int		i;		/* Loop variable over all chromosomes */
  int		chromo;		/* Index of chromosome to mutate */
  int		point;		/* Index of 'nucleotide' to mutate */

/* Checks */
  if (!father || !son) die("Null pointer to entity structure passed");

/* Select mutation locus. */
  chromo = (int) random_int(pop->num_chromosomes);
  point = (int) random_int(pop->len_chromosomes);

/* Copy unchanging data. */
  for (i=0; i<pop->num_chromosomes; i++)
    {
    memcpy(son->chromosome[i], father->chromosome[i], pop->len_chromosomes*sizeof(double));
    if (i!=chromo)
      {
      ga_copy_data(pop, son, father, i);
      }
    else
      {
      ga_copy_data(pop, son, NULL, i);
      }
    }

  ((double *)son->chromosome[chromo])[point] = random_unit_gaussian();

  return;
  }


