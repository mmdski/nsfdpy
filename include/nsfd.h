#ifndef NSFD_H_
#define NSFD_H_

#include <stddef.h>

typedef double NSFDReal;

size_t nsfd_sizeof_real (void);

// geometry data
typedef struct
{
  size_t   imax;
  size_t   jmax;
  NSFDReal xlength;
  NSFDReal ylength;
} NSFDGridGeomData;

// grid geometry API
typedef struct NSFDGridGeom NSFDGridGeom;

NSFDGridGeom *nsfd_grid_geom_new (NSFDGridGeomData *geom_data_p);
void          nsfd_grid_geom_free (NSFDGridGeom **grid_geom_pp);
size_t        nsfd_grid_geom_n_cells (NSFDGridGeom *grid_geom_p);
void          nsfd_grid_geom_init_p (NSFDGridGeom *grid_geom_p);
void          nsfd_grid_geom_init_u (NSFDGridGeom *grid_geom_p);
void          nsfd_grid_geom_init_v (NSFDGridGeom *grid_geom_p);
void
nsfd_grid_geom_coords (NSFDGridGeom *grid_geom_p, NSFDReal *x, NSFDReal *y);

// grid values API
typedef struct NSFDGridValues NSFDGridValues;

NSFDGridValues *nsfd_grid_values_new (NSFDGridGeomData *geom_data_p);
void            nsfd_grid_values_free (NSFDGridValues **grid_values_p);
size_t          nsfd_grid_values_n_cells (NSFDGridValues *grid_values_p);
void            nsfd_grid_values_init (NSFDGridValues *grid_values_p,
                                       NSFDReal        init_values);
void     nsfd_grid_values (NSFDGridValues *grid_values_p, NSFDReal *values);
NSFDReal nsfd_grid_values_abs_max (NSFDGridValues *values_p);

// time stepping data
typedef struct
{
  NSFDReal t;
  NSFDReal t_end;
  NSFDReal delt;
  NSFDReal tau;
} NSFDTimeStepData;

// pressure-iteration data
typedef struct
{
  size_t   itermax; // maximal number of pressure iterations in one time step
  size_t   it;      // SOR iteration counter
  NSFDReal res;     // norm of pressure equation residual
  NSFDReal eps;     // stopping tolerance \f$\epsilon\f$ for pressure iteration
  NSFDReal omg;     // relaxation parameter \f$\omega\f$ for SOR iteration
  NSFDReal gamma;   // upwind differencing factor \f$\gamma\f$
} NSFDPressIterData;

// problem-dependent quantities
typedef struct
{
  NSFDReal Re; // Reynolds number \f$Re\f$
  NSFDReal gx; // x-direction body force
  NSFDReal gy; // y-direction body force
} NSFDProblemData;

typedef struct
{
  NSFDReal ui; // initial x-direction velocity
  NSFDReal vi; // initial y-direction velocity
  NSFDReal pi; // initial pressure
} NSFDInitValData;

typedef enum
{
  kBCondNone,
  kBCondFreeSlip,
  kBCondNoSlip,
  kBCondOutflow,
  kBCondPeriodic
} NSFDBCondType;

typedef struct
{
  NSFDBCondType wW; // western (left) boundary
  NSFDBCondType wE; // eastern (right) boundary
  NSFDBCondType wN; // northern (upper) boundary
  NSFDBCondType wS; // southern (lower) boundary
} NSFDBCondData;

// initialize velocities and pressure
void nsfd_init_uvp (NSFDInitValData *iv_data_p,
                    NSFDGridValues  *u_p,
                    NSFDGridValues  *v_p,
                    NSFDGridValues  *p_p);

// compute time step
NSFDReal nsfd_comp_delt (NSFDTimeStepData *ts_data_p,
                         NSFDProblemData  *prob_data_p,
                         NSFDGridValues   *u_p,
                         NSFDGridValues   *v_p);

// set boundary conditions
void nsfd_set_b_cond (NSFDGridValues *u_p,
                      NSFDGridValues *v_p,
                      NSFDGridValues *p_p,
                      NSFDBCondData  *b_cond_data_p);
#endif
