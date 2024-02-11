#ifndef NSFD_MEM_H_
#define NSFD_MEM_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef PY_BUILD
#define PY_SSIZE_T_CLEAN
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#endif

static inline void *
nsfd_mem_malloc (size_t size)
{
  void *ptr = NULL;
#ifdef PY_BUILD
  ptr = PyMem_RawMalloc (size);
  if (ptr == NULL)
    {
      perror ("PyMem_RawMalloc");
      exit (EXIT_FAILURE);
    }
#else
  errno = 0;
  ptr   = malloc (size);
  if (ptr == NULL || errno != 0)
    {
      perror ("malloc");
      exit (EXIT_FAILURE);
    }
#endif
  memset (ptr, 0, size);

  return ptr;
}

static inline void
nsfd_mem_free (void **ptr_p)
{
#ifdef PY_BUILD
  PyMem_RawFree (*ptr_p);
#else
  free (*ptr_p);
#endif
  *ptr_p = NULL;
}

#endif
