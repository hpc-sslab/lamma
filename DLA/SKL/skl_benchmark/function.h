#ifndef FUNCTION_H
  #define FUNCTION_H
#include <stdlib.h>

int readArgument(kernel_t* bestKernel, char* arg);
int writeArgument(kernel_t* kernel, char* arg, int argValue);
kernel_t** climbinghill_2D(kernel_t *bestKernel, char* arg1, int arg1_range, int arg1_inc,
   char* arg2, int arg2_range, int arg2_inc, double buffer, int *count);

#endif
