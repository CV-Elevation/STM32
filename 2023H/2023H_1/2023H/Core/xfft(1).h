#ifndef	_xFFT_H
#define _xFFT_H

#include "main.h"
#define FFT_LENGTH 1024		//

typedef struct
{
	float real;
	float imag;
} Complex;

void FFT_Init(void);

unsigned char FFT(Complex *x,int m);
extern ALIGN_32BYTES (Complex q[FFT_LENGTH])   __attribute__((section(".ARM.__at_0x24000000")) );
extern ALIGN_32BYTES (Complex q1[FFT_LENGTH])  __attribute__((section(".ARM.__at_0x24040000")) );


void FFT_DIS(void);
void FFT_Get_frequency(void);

#endif
