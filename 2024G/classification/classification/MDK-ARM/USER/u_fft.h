#ifndef _U_FFT_H_
#define _U_FFT_H_

typedef struct
{
	float real;
	float imag;
} Complex;

extern float pAmp[512];
extern Complex data[1024];

unsigned char FFT(Complex *x, int m);
void u_fft_demo(float *src);


#endif