/**********************************************************************
  struggle2.c
 **********************************************************************

  struggle2 - Test/example program for GAUL.
  Copyright ©2001-2004, Stewart Adcock <stewart@linux-domain.com>
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

  Synopsis:	Test/example program for GAUL.

		This program demonstrates the alternative evolutionary
		schemes available within GAUL:
		o Darwinian Evolution.
		o Baldwinian Evolution.
		o Lamarckian Evolution.

		This program aims to generate the final sentence from
		Chapter 3 of Darwin's "The Origin of Species",
		entitled "Struggle for Existence".

		This example is explained in docs/html/tutorial2.html

 **********************************************************************/

/*
 * Includes
 */
#include "gaul.h"

/*
 * The solution string.
 */
static char *target_text="When we reflect on this struggle, we may console ourselves with the full belief, that the war of nature is not incessant, that no fear is felt, that death is generally prompt, and that the vigorous, the healthy, and the happy survive and multiply.";

/**********************************************************************
  struggle_score()
  synopsis:	Score solution.
  parameters:
  return:
  updated:	16/06/01
 **********************************************************************/

static boolean struggle_score(population *pop, entity *entity)
  {
  int		k;		/* Loop variable over all alleles. */

  entity->fitness = 0.0;

  /* Loop over alleles in chromosome. */
  for (k = 0; k < pop->len_chromosomes; k++)
    {
    if ( ((char *)entity->chromosome[0])[k] == target_text[k])
      entity->fitness+=1.0;
    /*
     * Component to smooth function, which helps a lot in this case:
     * Comment it out if you like.
     */
    entity->fitness += (127.0-abs((int)(((char *)entity->chromosome[0])[k]-target_text[k])))/50.0;
    }

  return TRUE;
  }


/**********************************************************************
  struggle_adaptation()
  synopsis:	Adapt solution by making a single hill-climbing step
		for a randomly selected allele.
  parameters:
  return:
  updated:	07/07/01
 **********************************************************************/

static entity *struggle_adaptation(population *pop, entity *child)
  {
  entity	*adult;		/* Adapted solution. */
  int		allele;		/* Randomly selected allele. */

/*
 * We must generate a new solution by copying the original solution.
 * This function copys all genomic, and if appropriate, phenomic data.
 * It is never safe to adapt the solution in place.
 */
  adult = ga_entity_clone(pop, child);

/* Make point mutation. */
  allele = random_int(strlen(target_text));
  ((char *)adult->chromosome[0])[allele]++;
  struggle_score(pop, adult);

  if (adult->fitness > child->fitness)
    return adult;

/* Searching in that previous direction didn't help. */
  ((char *)adult->chromosome[0])[allele]-=2;
  struggle_score(pop, adult);

  if (adult->fitness > child->fitness)
    return adult;

/* We must already be at a maxima. */
  ((char *)adult->chromosome[0])[allele]++;
  adult->fitness = child->fitness;

  return adult;
  }


/**********************************************************************
  main()
  synopsis:	Erm?
  parameters:
  return:
  updated:	19 Aug 2002
 **********************************************************************/

int main(int argc, char **argv)
  {
  population	*popd=NULL;		/* Population for Darwinian evolution. */
  population	*popb=NULL;		/* Population for Baldwinian evolution. */
  population	*popl=NULL;		/* Population for Lamarckian evolution. */
  char		*beststring=NULL;	/* Human readable form of best solution. */
  size_t	beststrlen=0;		/* Length of beststring. */

  random_seed(23091975);

  popd = ga_genesis_char(
     150,			/* const int              population_size */
     1,				/* const int              num_chromo */
     (int) strlen(target_text),	/* const int              len_chromo */
     NULL,		 	/* GAgeneration_hook      generation_hook */
     NULL,			/* GAiteration_hook       iteration_hook */
     NULL,			/* GAdata_destructor      data_destructor */
     NULL,			/* GAdata_ref_incrementor data_ref_incrementor */
     struggle_score,		/* GAevaluate             evaluate */
     ga_seed_printable_random,	/* GAseed                 seed */
     struggle_adaptation,	/* GAadapt                adapt */
     ga_select_one_sus,		/* GAselect_one           select_one */
     ga_select_two_sus,		/* GAselect_two           select_two */
     ga_mutate_printable_singlepoint_drift,	/* GAmutate    mutate */
     ga_crossover_char_allele_mixing,	/* GAcrossover         crossover */
     NULL,			/* GAreplace		replace */
     NULL			/* vpointer		User data */
            );

  ga_population_set_parameters(
     popd,			/* population   *pop */
     GA_SCHEME_DARWIN,		/* const ga_scheme_type scheme */
     GA_ELITISM_PARENTS_DIE,	/* const ga_elitism_type   elitism */
     0.9,			/* const double       crossover */
     0.1,			/* const double       mutation */
     0.0			/* const double       migration */
                            );

/*
 * Make exact copies of the populations, except modify
 * their evolutionary schemes.
 */
  popb = ga_population_clone(popd);
  ga_population_set_scheme(popb, GA_SCHEME_BALDWIN_CHILDREN);
  popl = ga_population_clone(popd);
  ga_population_set_scheme(popl, GA_SCHEME_LAMARCK_CHILDREN);

/*
 * Evolve each population in turn.
 */

  ga_evolution(
    popd,			/* population          *pop */
    600				/* const int           max_generations */
            );

  printf( "The final solution with Darwinian evolution with score %f was:\n",
          ga_get_entity_from_rank(popd,0)->fitness );
  beststring = ga_chromosome_char_to_string(popd, ga_get_entity_from_rank(popd,0), beststring, &beststrlen);
  printf("%s\n", beststring);

  ga_evolution(
    popb,			/* population          *pop */
    300				/* const int           max_generations */
            );

  printf( "The final solution with Baldwinian evolution with score %f was:\n",
          ga_get_entity_from_rank(popb,0)->fitness );
  beststring = ga_chromosome_char_to_string(popb, ga_get_entity_from_rank(popb,0), beststring, &beststrlen);
  printf("%s\n", beststring);

  ga_evolution(
    popl,			/* population          *pop */
    300				/* const int           max_generations */
            );

  printf( "The final solution with Lamarckian evolution with score %f was:\n",
          ga_get_entity_from_rank(popl,0)->fitness );
  beststring = ga_chromosome_char_to_string(popl, ga_get_entity_from_rank(popl,0), beststring, &beststrlen);
  printf("%s\n", beststring);

  /* Deallocate population structures. */
  ga_extinction(popd);
  ga_extinction(popb);
  ga_extinction(popl);

  /* Deallocate string buffer. */
  s_free(beststring);

  exit(EXIT_SUCCESS);
  }


