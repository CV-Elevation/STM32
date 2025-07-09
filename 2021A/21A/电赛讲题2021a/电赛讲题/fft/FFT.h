#include "stdlib.h"
#include "math.h"

typedef struct
{
	float real;
	float imag;
} Complex;

unsigned char FFT(Complex *x,int m);
