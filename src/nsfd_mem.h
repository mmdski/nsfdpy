#ifndef NSFD_MEM_H_
#define NSFD_MEM_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline void *
nsfd_mem_malloc (size_t size)
{
  errno     = 0;
  void *ptr = malloc (size);
  if (ptr == NULL || errno != 0)
    {
      perror ("malloc");
      exit (EXIT_FAILURE);
    }
  memset (ptr, 0, size);

  return ptr;
}

static inline void
nsfd_mem_free (void **ptr_p)
{
  free (*ptr_p);
  *ptr_p = NULL;
}

#endif
