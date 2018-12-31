/**********************************************************************
  helga_slang.c
 **********************************************************************

  helga_slang - S-Lang scripting in Helga.
  Copyright ©2001-2002, Stewart Adcock <stewart@linux-domain.com>

  The latest version of this program should be available at:
  http://www.stewart-adcock.co.uk/

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

  Synopsis:	Embed the S-Lang scripting engine into helga.

		This will be thread-safe if S-Lang is thread-safe.

  Updated:	08 Mar 2002 SAA	Added some extra functions to the intrinsics table.
		12 Feb 2002 SAA	Added a handful of intrinsic variable definitions.  Removed some dead code.
		06/05/01 SAA	SASA and CAD utility functions added.  helga_population_define_callbacks() renamed to helga_define_default_callbacks().
		24/03/01 SAA	Yet more intrinsics added.
		13/03/01 SAA	Added a handful of new intrinsic definitions.  Some tidying.
		28/02/01 SAA	Added linked list stuff to the test and diagnostic functions.
		27/02/01 SAA	Removed glib depedancy.
		02/02/01 SAA	Changes required by re-organisation of ga_util, ga_intrinsics, helga_qsort, helga_mpi and helga_log.
		31/01/01 SAA	Intrinsic "ga_genesis_canned" renamed to "helga_genesis_canned".
		28/01/01 SAA	Added to the helga_random wrappers.
		22/01/01 SAA	Multiple population handling moved out to the ga_intrinsics.c file.  Proper wrapper functions now used for the functions int helga_random.c and ga_util/intrinsics.c.
		19/01/01 SAA	More wrapper functions added.  Stuff necessary for unsing SLang functions through analysis hooks.  Fixed helga_slang_interpret_stdin().
		18/01/01 SAA	First version.

 **********************************************************************/

#include "helga_slang.h"

/*
 * Global variables.
 */
THREAD_LOCK_DEFINE_STATIC(slang_isinit_lock);
boolean slang_isinit=FALSE;	/* Whether SLang has been initialised. */

/*
 * Function wrappers:
 * (Helga's SLang intrinsic function definitions)
 */

double intrin_min(double *a, double *b)
  {
  if (*a < *b) return *a;
  return *b;
  }


void intrin_dump(char *arg)
  {
  if (strcmp(arg, "backbone") == 0)
    helga_show_backbone_coords();
  else if (strcmp(arg, "rotamers") == 0)
    helga_show_rotamers();
  else if (strcmp(arg, "sequences") == 0)
    helga_write_sequences();
  else if (strcmp(arg, "oversized_sequences") == 0)
    helga_write_oversized_sequences();
/* Deprecated.
  else if (strcmp(arg, "parameters") == 0)
    helga_write_params(NULL);
*/
  else if (strcmp(arg, "constraints") == 0)
    helga_show_constraints();

  return;
  }

void intrin_test(char *arg)
  {
  if (strcmp(arg, "avltree") == 0)
    avltree_test();
  else if (strcmp(arg, "encode") == 0)
    helga_encode_test();
  else if (strcmp(arg, "lists") == 0)
    linkedlist_test();
  else if (strcmp(arg, "mpi") == 0)
    mpi_test();
  else if (strcmp(arg, "random") == 0)
    random_test();
  else if (strcmp(arg, "rotate") == 0)
    rotate_test();
  else if (strcmp(arg, "table") == 0)
    table_test();
  else if (strcmp(arg, "qsort") == 0)
    ga_qsort_test();
/*
  else if (strcmp(arg, "chunks") == 0)
    mem_chunk_test();
*/

  return;
  }

void intrin_diagnostics(char *arg)
  {
  if (strcmp(arg, "avltree") == 0)
    avltree_diagnostics();
  else if (strcmp(arg, "dmatrix") == 0)
    dmatrix_diagnostics();
  else if (strcmp(arg, "dstring") == 0)
    dstr_diagnostics();
  else if (strcmp(arg, "ga") == 0)
    ga_diagnostics();
  else if (strcmp(arg, "lists") == 0)
    linkedlist_diagnostics();
/*
  else if (strcmp(arg, "chunks") == 0)
    mem_chunk_diagnostics();
*/
/*
  else if (strcmp(arg, "mpi") == 0)
    mpi_diagnostics();
*/
  else if (strcmp(arg, "random") == 0)
    random_diagnostics();
  else if (strcmp(arg, "rotate") == 0)
    rotate_diagnostics();
  else if (strcmp(arg, "table") == 0)
    table_diagnostics();
  else if (strcmp(arg, "vector3") == 0)
    v3_diagnostics();

  return;
  }


/**********************************************************************
  helga_say_bye_bye()
  synopsis:	Gracefully die.
  parameters:	none
  return:	none
  last updated:	28/02/01
 **********************************************************************/

void helga_say_bye_bye(void)
  {
  /*mpi_exit();*/
  exit(2);	/* Return value of '2' for profiling tool. */
  }


/*
 * Helga's SLang Interface:
 */

/**********************************************************************
  helga_slang_call()
  synopsis:	Call a user-defined or intrinisic SLang function.
  parameters:	char *func_name	The function's name.
  return:	Success/Failure.
  last updated:	19/01/01
 **********************************************************************/

boolean helga_slang_call(char *func_name)
  {

/*
 * int	type=SLang_is_defined(func_name);
 * if (type < 1) return FALSE;
 * SLang_is_defined() returns:
 *   0    no defined object
 *   1    intrinsic function
 *   2    user-defined slang function
 *  -1    intrinsic variable
 *  -2    user-defined global variable
 */

  if (SLang_execute_function(func_name)>0) return TRUE;

  return FALSE;
  }


/**********************************************************************
  helga_slang_isinit()
  synopsis:	Return TRUE if slang has been initialised.
  parameters:	none
  return:	boolean
  last updated:	19/01/01
 **********************************************************************/

boolean helga_slang_isinit(void)
  {
  return slang_isinit;
  }


/**********************************************************************
  helga_slang_init()
  synopsis:	Initialise SLang scripting engine.
  parameters:	none
  return:	Success/Failure.
  last updated:	08 Mar 2002
 **********************************************************************/

boolean helga_slang_init(void)
  {
  boolean	go=TRUE;			/* Block multiple initialisations. */
  static int	true_var=TRUE, false_var=FALSE;	/* Intrinsic variables. */
  static int	log0=LOG_NONE, log1=LOG_FATAL, log2=LOG_WARNING, log3a=LOG_QUIET,
		log3b=LOG_NORMAL, log4=LOG_VERBOSE, log5=LOG_FIXME, log6=LOG_DEBUG;

  THREAD_LOCK(slang_isinit_lock);
  if (slang_isinit) 
    go=FALSE;
  else
    slang_isinit=TRUE;
  THREAD_UNLOCK(slang_isinit_lock);

  if (!go)
    {
    helga_log(LOG_VERBOSE, "SLang has already been initialised.");
    return TRUE;
    }

  helga_log(LOG_VERBOSE, "Initialising SLang.");

/*
 * Enable SLang interpreter with all intrinsic functions.
 * A lower overhead version would probably need:
 * if ( -1 == SLang_init_slang()  ||
 *      -1 == SLang_init_slmath() ||
 *      -1 == SLang_init_stdio()     ) return FALSE;
 */
  if (-1 == SLang_init_all()) return FALSE;

/*
 * General tests.
 */
  if ( SLadd_intrinsic_function("min", (FVOID_STAR) intrin_min,
                    SLANG_DOUBLE_TYPE, 2, SLANG_DOUBLE_TYPE, SLANG_DOUBLE_TYPE)
      || SLadd_intrinsic_function("dump", (FVOID_STAR) intrin_dump,
                    SLANG_VOID_TYPE, 1, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("test", (FVOID_STAR) intrin_test,
                    SLANG_VOID_TYPE, 1, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("diagnostics", (FVOID_STAR) intrin_diagnostics,
                    SLANG_VOID_TYPE, 1, SLANG_STRING_TYPE)
     ) return FALSE;

/*
 * Genetic Algorithm interface stuff: The libGA_util intrinsics.
 * These intrinsic functions are defined in ga_intrinsics.c (Although they are
 * mostly just simple wrapper functions.)
 */
  if ( ga_intrinsic_sladd()==FALSE ) return FALSE;

/*
 * Logging facilities.
 */
  if (   SLadd_intrinsic_function("helga_log_init",
             (FVOID_STAR) log_init, SLANG_VOID_TYPE, 1,
             SLANG_INT_TYPE)
     ) return FALSE;

/*
 * Rotamer library handling.
 */
  if (   SLadd_intrinsic_function("helga_init_rotamers_crd",
             (FVOID_STAR) helga_init_rotamers, SLANG_VOID_TYPE, 3,
             SLANG_STRING_TYPE, SLANG_STRING_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_init_rotamers_pdb",
             (FVOID_STAR) helga_init_rotamers_pdb, SLANG_VOID_TYPE, 3,
             SLANG_STRING_TYPE, SLANG_STRING_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_clear_rotamers",
             (FVOID_STAR) helga_init_rotamers, SLANG_VOID_TYPE, 0)
     ) return FALSE;

/*
 * Helix analysis and manipulation.
 */
  if (   SLadd_intrinsic_function("helga_display_packing",
             (FVOID_STAR) helga_display_packing_wrapper, SLANG_INT_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_display_helix_parameters",
             (FVOID_STAR) helga_display_helixparams_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_display_ic_deviations",
             (FVOID_STAR) helga_display_ic_deviations_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_display_residue_config",
             (FVOID_STAR) helga_display_helix_dihed_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_display_backbone_dihedrals",
             (FVOID_STAR) helga_display_backbone_dihedrals_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_direction_plot",
             (FVOID_STAR) helga_write_direction_plot_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_residue_z_distribution",
             (FVOID_STAR) helga_write_residue_z_distribution_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_residue_residue_distribution",
             (FVOID_STAR) helga_write_all_residue_distribution_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_residue_sasa_distribution",
             (FVOID_STAR) helga_write_sasa_distribution_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_residue_angular_distribution",
             (FVOID_STAR) helga_write_residue_angle_distribution_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_moment_plot",
             (FVOID_STAR) helga_write_moment_plot_wrapper, SLANG_VOID_TYPE, 4,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_exposed_plot",
             (FVOID_STAR) helga_write_exposure_plot_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_outward_plot",
             (FVOID_STAR) helga_write_outward_plot_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_contact_areas_pdb",
             (FVOID_STAR) helga_analyse_contact_areas_pdb, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_cae",
             (FVOID_STAR) helga_analyse_entities_cae_wrapper, SLANG_DOUBLE_TYPE, 4,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_cae_pdb",
             (FVOID_STAR) helga_analyse_pdb_cae, SLANG_DOUBLE_TYPE, 3,
             SLANG_STRING_TYPE, SLANG_STRING_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_rmsd",
             (FVOID_STAR) helga_analyse_rmsd_wrapper, SLANG_DOUBLE_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_ca_rmsd",
             (FVOID_STAR) helga_analyse_ca_rmsd_wrapper, SLANG_DOUBLE_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_rmsd_pdb",
             (FVOID_STAR) helga_analyse_pdb_rmsd, SLANG_DOUBLE_TYPE, 2,
             SLANG_STRING_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_ca_rmsd_pdb",
             (FVOID_STAR) helga_analyse_pdb_ca_rmsd, SLANG_DOUBLE_TYPE, 2,
             SLANG_STRING_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_contact_areas",
             (FVOID_STAR) helga_analyse_entity_contact_areas_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_sasa",
             (FVOID_STAR) helga_analyse_entity_sasa_wrapper, SLANG_DOUBLE_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_write_som_pak_data",
             (FVOID_STAR) helga_write_som_pak_data_wrapper, SLANG_VOID_TYPE, 5,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE, SLANG_STRING_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_write_density_map",
             (FVOID_STAR) helga_write_density_plot_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_helix_rotate",
             (FVOID_STAR) helga_helix_rotate_wrapper, SLANG_VOID_TYPE, 4,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
     ) return FALSE;

/*
 * Sequence handling stuff.
 * Some of these should be deprecated.
 */
  if (   SLadd_intrinsic_function("add_sequence",
             (FVOID_STAR) helga_register_sequence_from_string, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE) /* FIXME */
      || SLadd_intrinsic_function("add_sequence_string",
             (FVOID_STAR) helga_register_sequence_from_string, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("add_sequence_from_seqres",
             (FVOID_STAR) helga_register_sequence_from_seqres, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("add_oversized_sequence",
             (FVOID_STAR) helga_register_oversized_sequence_from_string, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE) /* FIXME */
      || SLadd_intrinsic_function("add_oversized_sequence_string",
             (FVOID_STAR) helga_register_oversized_sequence_from_string, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("num_sequences",
             (FVOID_STAR) helga_get_sequence_count, SLANG_INT_TYPE, 0) /* Deprecate. */
      || SLadd_intrinsic_function("len_sequence",
             (FVOID_STAR) helga_get_sequence_size, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE) /* Deprecate. */
      || SLadd_intrinsic_function("total_len_sequence",
             (FVOID_STAR) helga_get_total_sequence_size, SLANG_INT_TYPE, 0) /* Deprecate. */
      || SLadd_intrinsic_function("clear_sequences",
             (FVOID_STAR) helga_clear_sequences, SLANG_VOID_TYPE, 0) /* Deprecate. */
      || SLadd_intrinsic_function("set_sequences_from_crd",
             (FVOID_STAR) helga_register_sequences_from_crd, SLANG_INT_TYPE, 1,
             SLANG_STRING_TYPE) /* Deprecate. */
      || SLadd_intrinsic_function("set_sequences_from_pdb",
             (FVOID_STAR) helga_register_sequences_from_pdb, SLANG_INT_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("adjust_sequence",
             (FVOID_STAR) helga_sequence_adjust_wrapper, SLANG_INT_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE) /* Deprecate. */
      || SLadd_intrinsic_function("helga_set_nterminal_patch",
             (FVOID_STAR) helga_set_nterminal_patch, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_set_cterminal_patch",
             (FVOID_STAR) helga_set_cterminal_patch, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_get_sequence_count",
             (FVOID_STAR) helga_get_sequence_count, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_get_sequence_size",
             (FVOID_STAR) helga_get_sequence_size, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_get_total_sequence_size",
             (FVOID_STAR) helga_get_total_sequence_size, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_clear_sequences",
             (FVOID_STAR) helga_clear_sequences, SLANG_VOID_TYPE, 0)
      || SLadd_intrinsic_function("helga_adjust_sequence",
             (FVOID_STAR) helga_sequence_adjust_wrapper, SLANG_INT_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
     ) return FALSE;

/*
 * Structure optimisation by deterministic methods.
 */
  if (   SLadd_intrinsic_function("helga_sd_rigid",
             (FVOID_STAR) helga_optimise_sd_rigid_wrapper, SLANG_INT_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
     ) return FALSE;

/*
 * Constraint handling and scoring stuff.
 * FIXME: Need a proper, sensible, interface to the constraints code.
 */
  if (   SLadd_intrinsic_function("add_constraint",
             (FVOID_STAR) helga_parse_constraint_string, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("add_constraint_string",
             (FVOID_STAR) helga_parse_constraint_string, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("constraint_clear_all",
             (FVOID_STAR) helga_free_constraints, SLANG_VOID_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_verbosity",
             (FVOID_STAR) helga_scoring_set_verbosity_wrapper, SLANG_VOID_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_reset_count",
             (FVOID_STAR) helga_scoring_reset_count, SLANG_VOID_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_get_count",
             (FVOID_STAR) helga_scoring_get_count, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_backbone",
             (FVOID_STAR) helga_score_get_need_backbone, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_sidechains",
             (FVOID_STAR) helga_score_get_need_sidechains, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_q",
             (FVOID_STAR) helga_score_get_need_q, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_com",
             (FVOID_STAR) helga_score_get_need_com, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_axis",
             (FVOID_STAR) helga_score_get_need_axis, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_cacoords",
             (FVOID_STAR) helga_score_get_need_cacoords, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_surface",
             (FVOID_STAR) helga_score_get_need_surface, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_need_chothia",
             (FVOID_STAR) helga_score_get_need_chothia, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("helga_scoring_set_need_backbone",
             (FVOID_STAR) helga_score_set_need_backbone_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_sidechains",
             (FVOID_STAR) helga_score_set_need_sidechains_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_q",
             (FVOID_STAR) helga_score_set_need_q_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_com",
             (FVOID_STAR) helga_score_set_need_com_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_axis",
             (FVOID_STAR) helga_score_set_need_axis_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_cacoords",
             (FVOID_STAR) helga_score_set_need_cacoords_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_surface",
             (FVOID_STAR) helga_score_set_need_surface_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_scoring_set_need_chothia",
             (FVOID_STAR) helga_score_set_need_chothia_s, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
     ) return FALSE;

/*
 * Random number stuff.
  if (   SLadd_intrinsic_function("random_init",
             (FVOID_STAR) random_init, SLANG_VOID_TYPE, 0)
      || SLadd_intrinsic_function("random_isinit",
             (FVOID_STAR) random_isinit, SLANG_INT_TYPE, 0)
 */
  if (   SLadd_intrinsic_function("random_get_state",
             (FVOID_STAR) random_get_state_str, SLANG_STRING_TYPE, 0)
      || SLadd_intrinsic_function("random_get_state_len",
             (FVOID_STAR) random_get_state_str_len, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("random_set_state",
             (FVOID_STAR) random_set_state_str, SLANG_VOID_TYPE, 1,
             SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("random_boolean",
             (FVOID_STAR) random_boolean, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("random_unit_uniform",
             (FVOID_STAR) random_unit_uniform, SLANG_DOUBLE_TYPE, 0)
      || SLadd_intrinsic_function("random_unit_gaussian",
             (FVOID_STAR) random_unit_gaussian, SLANG_DOUBLE_TYPE, 0)
      || SLadd_intrinsic_function("random_unit_gaussian",
             (FVOID_STAR) random_unit_gaussian, SLANG_DOUBLE_TYPE, 0)
      || SLadd_intrinsic_function("random_rand",
             (FVOID_STAR) random_rand_wrapper, SLANG_INT_TYPE, 0)
      || SLadd_intrinsic_function("random_tseed",
             (FVOID_STAR) random_tseed, SLANG_VOID_TYPE, 0)
      || SLadd_intrinsic_function("random_seed",
             (FVOID_STAR) random_seed_wrapper, SLANG_VOID_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("random_boolean_prob",
             (FVOID_STAR) random_boolean_prob_wrapper, SLANG_INT_TYPE, 1,
             SLANG_DOUBLE_TYPE)
      || SLadd_intrinsic_function("random_int",
             (FVOID_STAR) random_int_wrapper, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("random_int_range",
             (FVOID_STAR) random_int_range_wrapper, SLANG_INT_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("random_double_range",
             (FVOID_STAR) random_double_range_wrapper, SLANG_DOUBLE_TYPE, 2,
             SLANG_DOUBLE_TYPE, SLANG_DOUBLE_TYPE)
      || SLadd_intrinsic_function("random_double",
             (FVOID_STAR) random_double_wrapper, SLANG_DOUBLE_TYPE, 1,
             SLANG_DOUBLE_TYPE)
      || SLadd_intrinsic_function("random_double_1",
             (FVOID_STAR) random_double_1, SLANG_DOUBLE_TYPE, 0)
      || SLadd_intrinsic_function("random_gaussian",
             (FVOID_STAR) random_gaussian_wrapper, SLANG_DOUBLE_TYPE, 2,
             SLANG_DOUBLE_TYPE, SLANG_DOUBLE_TYPE)
     ) return FALSE;

/*
 * Log facility.
 */
  if (   SLadd_intrinsic_function("helga_log",
            (FVOID_STAR) log_wrapper, SLANG_VOID_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
     ) return FALSE;

/*
 * HelGA specific GA/population/entity stuff.
 */
  if (   SLadd_intrinsic_function("helga_write_entity_crd",
            (FVOID_STAR) helga_ga_write_entity_crd_wrapper, SLANG_INT_TYPE, 3,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_write_entity_pdb",
            (FVOID_STAR) helga_ga_write_entity_pdb_wrapper, SLANG_INT_TYPE, 3,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_population_new",
            (FVOID_STAR) helga_ga_population_new, SLANG_INT_TYPE, 1,
            SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_genesis_canned",
            (FVOID_STAR) helga_genesis_canned, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_define_default_callbacks",
            (FVOID_STAR) helga_ga_define_callbacks_wrapper, SLANG_INT_TYPE, 1,
            SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_seed_op",
            (FVOID_STAR) helga_ga_set_seed_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_select1_op",
            (FVOID_STAR) helga_ga_set_select1_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_select2_op",
            (FVOID_STAR) helga_ga_set_select2_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_adapt_op",
            (FVOID_STAR) helga_ga_set_adapt_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_crossover_op",
            (FVOID_STAR) helga_ga_set_crossover_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_mutate_op",
            (FVOID_STAR) helga_ga_set_mutate_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_set_replace_op",
            (FVOID_STAR) helga_ga_set_replace_op, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_reencode",
            (FVOID_STAR) helga_reencode, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_reencode_rr",
            (FVOID_STAR) helga_reencode_simple, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_reencode_verbatim_data",
            (FVOID_STAR) helga_reencode_copy_data, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_reencode_self",
            (FVOID_STAR) helga_reencode_self, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_encode_native_pdb",
            (FVOID_STAR) helga_encode_native_data_pdb, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_native_crd",
            (FVOID_STAR) helga_encode_native_data_crd, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_asis_pdb",
            (FVOID_STAR) helga_encode_asis_data_pdb, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_asis_crd",
            (FVOID_STAR) helga_encode_asis_data_crd, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_rr_pdb",
            (FVOID_STAR) helga_encode_simple_pdb, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_rr_crd",
            (FVOID_STAR) helga_encode_simple_crd, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_pdb",
            (FVOID_STAR) helga_encode_pdb, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_encode_pdb_model",
            (FVOID_STAR) helga_encode_pdb_model, SLANG_INT_TYPE, 3,
            SLANG_INT_TYPE, SLANG_STRING_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_encode_crd",
            (FVOID_STAR) helga_encode_crd, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_entity_display_data_summary",
            (FVOID_STAR) helga_entity_display_data_summary_wrapper, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_entity_display_gene_summary",
            (FVOID_STAR) helga_entity_display_gene_summary_wrapper, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_entity_display_decoded_summary",
            (FVOID_STAR) helga_entity_display_decoded_summary_wrapper, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_lookup_residue_type",
            (FVOID_STAR) helga_lookup_residue_type, SLANG_INT_TYPE, 1,
            SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_lookup_residue_symbol",
            (FVOID_STAR) helga_lookup_residue_symbol, SLANG_INT_TYPE, 1,
            SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_sequence_mutate",
            (FVOID_STAR) helga_sequence_mutate_wrapper, SLANG_INT_TYPE, 4,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_orientation_search",
            (FVOID_STAR) helga_ga_orientation_search_wrapper, SLANG_INT_TYPE, 3,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_translation_search",
            (FVOID_STAR) helga_ga_translation_search_wrapper, SLANG_INT_TYPE, 6,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_DOUBLE_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_conformation_search",
            (FVOID_STAR) helga_ga_conformation_search_wrapper, SLANG_INT_TYPE, 4,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_mutate_sequence_to_string",
            (FVOID_STAR) helga_ga_mutate_helix_identity_wrapper, SLANG_INT_TYPE, 4,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_thread_helix_sequence_string",
            (FVOID_STAR) helga_sequence_helix_thread_wrapper, SLANG_INT_TYPE, 4,
            SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_thread_sequence",
            (FVOID_STAR) helga_sequence_thread_wrapper, SLANG_INT_TYPE, 2,
            SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_score_native_pdb",
            (FVOID_STAR) helga_score_native_pdb, SLANG_DOUBLE_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_score_native_crd",
            (FVOID_STAR) helga_score_native_crd, SLANG_DOUBLE_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_score_asis_pdb",
            (FVOID_STAR) helga_score_asis_pdb, SLANG_DOUBLE_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_score_asis_crd",
            (FVOID_STAR) helga_score_asis_crd, SLANG_DOUBLE_TYPE, 2,
            SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_align",
             (FVOID_STAR) helga_entity_align_bundle_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_entity_seed",
             (FVOID_STAR) helga_ga_seed_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_entity_seed_pore",
             (FVOID_STAR) helga_ga_seed_pore_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_entity_seed_random",
             (FVOID_STAR) helga_ga_seed_random_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_entity_score_residue_contributions",
             (FVOID_STAR) helga_score_residue_contributions_entity_wrapper, SLANG_VOID_TYPE, 3,
             SLANG_INT_TYPE, SLANG_INT_TYPE, SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_entity_build_missing_atoms",
             (FVOID_STAR) helga_build_missing_atoms, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_clear_scores",
             (FVOID_STAR) helga_clear_scores_wrapper, SLANG_VOID_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_idealise",
             (FVOID_STAR) helga_idealise_wrapper, SLANG_VOID_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_chromosome_get_num_alleles",
             (FVOID_STAR) helga_chromosome_get_num_alleles_slang, SLANG_INT_TYPE, 1,
             SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_get_num_alleles",
             (FVOID_STAR) helga_get_num_alleles, SLANG_INT_TYPE, 1,
             SLANG_VOID_TYPE)
      || SLadd_intrinsic_function("helga_entity_evaluate",
             (FVOID_STAR) helga_entity_evaluate_slang, SLANG_DOUBLE_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
     ) return FALSE;

/* Dead-end elimination stuff. */
  if (   SLadd_intrinsic_function("helga_dee_rotamer_search",
            (FVOID_STAR) helga_dee_rotamer_search_wrapper, SLANG_INT_TYPE, 2,
             SLANG_INT_TYPE, SLANG_INT_TYPE)
      || SLadd_intrinsic_function("helga_dee_test",
             (FVOID_STAR) helga_dee_test, SLANG_VOID_TYPE, 0)
     ) return FALSE;

/* Plug-in modules. */
  if (   SLadd_intrinsic_function("helga_plugin_open",
            (FVOID_STAR) helga_plugin_open, SLANG_INT_TYPE, 1,
            SLANG_STRING_TYPE)
      || SLadd_intrinsic_function("helga_plugin_close",
            (FVOID_STAR) helga_plugin_close, SLANG_INT_TYPE, 1,
            SLANG_STRING_TYPE)
     ) return FALSE;

/* Memory debugging. */
  if (   SLadd_intrinsic_function("memory_display_table",
            (FVOID_STAR) memory_display_table, SLANG_VOID_TYPE, 0)
      || SLadd_intrinsic_function("memory_display_status",
            (FVOID_STAR) memory_display_status, SLANG_VOID_TYPE, 0)
     ) return FALSE;

/* (Almost) Graceful quit. */
  if (   SLadd_intrinsic_function("helga_finalize",
            (FVOID_STAR) helga_say_bye_bye, SLANG_VOID_TYPE, 0)
     ) return FALSE;

/*
 * Define some read-only intrinsic variables.
 */
  if (   SLadd_intrinsic_variable("TRUE", &true_var, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("FALSE", &false_var, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_NONE", &log0, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_FATAL", &log1, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_WARNING", &log2, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_QUIET", &log3a, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_NORMAL", &log3b, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_VERBOSE", &log4, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_FIXME", &log5, SLANG_INT_TYPE, TRUE)
      || SLadd_intrinsic_variable("LOG_DEBUG", &log6, SLANG_INT_TYPE, TRUE)
     ) return FALSE;

  return TRUE;
  }


/**********************************************************************
  helga_slang_interpret_file()
  synopsis:
  parameters:
  return:
  last updated:	18/01/01
 **********************************************************************/

boolean helga_slang_interpret_file(char *script_fname)
  {

  if (!script_fname) die("NULL pointer to filename passed.");

  helga_log(LOG_VERBOSE, "Interpreting SLang script \"%s\".", script_fname);

/*
 * Load SLang script.
 */
  if (-1 == SLang_load_file(script_fname))
    {
    dief("Error %d interpreting the SLang script \"%s\".",
         SLang_Error, script_fname);
    SLang_restart(1);	/* reset interpreter. */
    SLang_Error = 0;
    return FALSE;
    }

  return TRUE;
  }


/**********************************************************************
  helga_slang_interpret_stdin()
  synopsis:
  parameters:
  return:
  last updated:	18/01/01
 **********************************************************************/

boolean helga_slang_interpret_stdin(void)
  {
  helga_log(LOG_VERBOSE, "Interpreting stdin as SLang.");

/*
 * Interprete SLang script from stdin.
 */
  if (-1 == SLang_load_file(NULL))
    {
    dief("Error %d interpreting the SLang script from stdin.", SLang_Error);
    SLang_restart(1);
    SLang_Error = 0;
    return FALSE;
    }

#if 0
/*
 * This is how the S-Lang documentation implies the above should be done.
 * -- but it doesn't work...
 */
  char	buffer[256];	/* Stdin buffer. */
  int	linecount=0;	/* Number of lines parsed. */

  while (fgets(buffer, 256, stdin))
    {
    printf("STDIN line %d\n", linecount);
    if (-1 == SLang_load_string(buffer))
      {
      printf("SLang_Error = %d\n", SLang_Error);
      SLang_restart (1);
      }
    SLang_Error = 0;
    linecount++;
    }
#endif

  return TRUE;
  }

