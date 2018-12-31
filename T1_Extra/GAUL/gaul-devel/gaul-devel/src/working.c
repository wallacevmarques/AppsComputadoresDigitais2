
/**********************************************************************
  ga_chromosome_bitstring_discrete_compare()
  synopsis:	Compare chromosomes.
  parameters:
                int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	TRUE if they are exactly identical.
  last updated: 22 Aug 2003
 **********************************************************************/

boolean ga_chromosome_bitstring_discrete_compare(
                              const population *pop,
                              const entity *alpha,
                              const entity *beta,
                              int *count, int *nab, int *na, int *nb, int *n )
  {
  int		i, j;		/* Loop over chromosome, alleles. */

  if (!pop) die("Null pointer to population structure passed.");
  if (!alpha || !beta) die("Null pointer to entity structure passed.");

  *count=0;	/* Total number of alleles. */
  *nab=0;	/* Number of alleles "on" in both chromosomes. */
  *na=0;		/* Number of alleles "on" in alpha only. */
  *nb=0;		/* Number of alleles "on" in beta only. */
  *n=0;		/* Number of alleles "off" in both chromosomes. */

  for(i=0; i<pop->num_chromosomes; i++)
    {
    for(j=0; j<pop->len_chromosomes; j++)
      {
      count++;
      if ( ga_bit_get(alpha->chromosome[i],j) )
        {
        if ( ga_bit_get(beta->chromosome[i],j) )
          {
          *nab++;
          }
        else
          {
          *na++;
          }
        }
      else if ( ga_bit_get(beta->chromosome[i],j) )
        {
        *nb++;
        }
      else
        {
        *n++;
        }
      }
    }

  return *nab==*count;
  }


/**********************************************************************
  ga_chromosome_boolean_discrete_compare()
  synopsis:	Compare chromosomes.
  parameters:
                int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	TRUE if they are exactly identical.
  last updated: 22 Aug 2003
 **********************************************************************/

boolean ga_chromosome_boolean_discrete_compare(
                              const population *pop,
                              const entity *alpha,
                              const entity *beta,
                              int *count, int *nab, int *na, int *nb, int *n )
  {
  int		i, j;		/* Loop over chromosome, alleles. */

  if (!pop) die("Null pointer to population structure passed.");
  if (!alpha || !beta) die("Null pointer to entity structure passed.");

  *count=0;	/* Total number of alleles. */
  *nab=0;	/* Number of alleles "on" in both chromosomes. */
  *na=0;		/* Number of alleles "on" in alpha only. */
  *nb=0;		/* Number of alleles "on" in beta only. */
  *n=0;		/* Number of alleles "off" in both chromosomes. */

  for(i=0; i<pop->num_chromosomes; i++)
    {
    for(j=0; j<pop->len_chromosomes; j++)
      {
      count++;
      if ( ((boolean *)alpha->chromosome[i])[j] == 1 )
        {
        if ( ((boolean *)beta->chromosome[i])[j] == 1 )
          {
          *nab++;
          }
        else
          {
          *na++;
          }
        }
      else if ( ((boolean *)beta->chromosome[i])[j] == 1 )
        {
        *nb++;
        }
      else
        {
        *n++;
        }
      }
    }

  return *nab==*count;
  }


/**********************************************************************
  ga_similarity_discrete_braycurtis()
  synopsis:	Calculate statistical association measure.
		Bray & Curtis method.

                            (Na+Nb)
                D[0,1] = -------------
		         2*Nab+(Na+Nb)

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_braycurtis(
                              int count, int nab, int na, int nb, int n )
  {
  double	dissim = (double)(na+nb)/(double)(2.0*nab+(na+nb));

  return 1.0-dissim;
  }


/**********************************************************************
  ga_similarity_discrete_match()
  synopsis:	Calculate statistical association measure.
		Simple matching method. Sokal and Mich. 1985.

                         (Nab+N)
                S[0,1] = -------
		            C

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_match(
                              int count, int nab, int na, int nb, int n )
  {

  return (double)(nab+n)/(double)count;
  }


/**********************************************************************
  ga_similarity_discrete_pearson()
  synopsis:	Calculate statistical association measure.
                Pearson correlation.

                                    (Nab*N)-(Na*Nb)
                S[-1,1] = ------------------------------------
		          ((Nab+Na)(Nab+Nb)(Na+N)(Nb+N))^(1/2)

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_pearson(
                              int count, int nab, int na, int nb, int n )
  {
  double	sim = ((double)(nab*n)-(na*nb))/sqrt((double)(nab+na)*(nab+nb)*(na+n)*(nb+n));

  return 0.5+(sim/2.0);
  }


/**********************************************************************
  ga_similarity_discrete_dice()
  synopsis:	Calculate statistical association measure.
                Czekanowski-Dice method.

                            2*Nab
                S[0,1] = -----------
		         2*Nab+Na+Nb

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_dice(
                              int count, int nab, int na, int nb, int n )
  {

  return (double)(2*nab)/(double)(2*nab+na+nb);
  }


/**********************************************************************
  ga_similarity_discrete_shape()
  synopsis:	Calculate statistical association measure.
                Binary shape difference method.

                         C*(Na+Nb)-(Na-Nb)^2
                D[0,1] = -------------------
		                 C^2

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_shape(
                              int count, int nab, int na, int nb, int n )
  {
  double	dissim = (double)(count*(na+nb)-(na-nb)*(na-nb))/(double)(count*count);

  return 1.0-dissim;
  }


/**********************************************************************
  ga_similarity_discrete_pattern()
  synopsis:	Calculate statistical association measure.
                Binary pattern difference method.

                         Na*Nb
                D[0,1] = -----
		          C^2

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_pattern(
                              int count, int nab, int na, int nb, int n )
  {
  double	dissim = (double)(na*nb)/(double)(count*count);

  return 1.0-dissim;
  }


/**********************************************************************
  ga_similarity_discrete_size()
  synopsis:	Calculate statistical association measure.
                Binary size difference method.

                         (Na+Nb)^2
                D[0,1] = ---------
		            C^2

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_size(
                              int count, int nab, int na, int nb, int n )
  {
  double	dissim = (double)((na+nb)*(na+nb))/(double)(count*count);

  return 1.0-dissim;
  }


/**********************************************************************
  ga_similarity_discrete_euclidean()
  synopsis:	Calculate statistical association measure.
                Euclidean distance.

                         Na+Nb
                D[0,1] = -----
		           C

  parameters:	int	count	Total number of alleles.
                int	nab	Number of alleles "on" in both chromosomes.
                int	na	Number of alleles "on" in alpha only.
                int	nb	Number of alleles "on" in beta only.
                int	n	Number of alleles "off" in both chromosomes.
  return:	double	similarity.
  last updated: 22 Aug 2003
 **********************************************************************/

static double ga_similarity_discrete_euclidean(
                              int count, int nab, int na, int nb, int n )
  {
  double	dissim = (double)(na+nb)/(double)(count*count);

  return 1.0-dissim;
  }
