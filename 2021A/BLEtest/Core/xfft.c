#include "xFFT.h"
#include "math.h"

ALIGN_32BYTES (Complex q[FFT_LENGTH])   __attribute__((section(".ARM.__at_0x24000000")) );
ALIGN_32BYTES (Complex q1[FFT_LENGTH])  __attribute__((section(".ARM.__at_0x24040000")) );

void FFT_DIS(void)
{
////		for(uint32_t i=1550; i<6750; i++)
////		for(uint32_t i = 0; i < FFT_LENGTH; i++) 
		for(uint32_t i=0; i<FFT_LENGTH/2; i++)
		{
			printf("%f\r\n", q1[i].real);
//			printf("%d:\t%f\r\n", i,  q1[i].real);			
		}
//求THD	
		float A_max =0;
		float A_1 =0;
		float A_2 =0;
		float A_3 =0;
		float A_4 =0;
		uint32_t i_Amax,i_Asec= 0;
		uint32_t i =0;
		for(i=1;i<=FFT_LENGTH/2;i++)
		{
			if(q1[i].real >= A_max&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_max = q1[i].real;
				i_Amax = i;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_1)&&(q1[i].real<A_max)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_1 = q1[i].real;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_2)&&(q1[i].real<A_1)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_2 = q1[i].real;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_3)&&(q1[i].real<A_2)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_3 = q1[i].real;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_4)&&(q1[i].real<A_3)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_4 = q1[i].real;
			}
		}
		float Fs = 240000000 / 1 / 80;					// 采样率，例如1000.0f 
		float f_max = i_Amax * Fs / FFT_LENGTH;
//	float f_sec = i_Asec * Fs / FFT_LENGTH;
//		printf("frequency = %fHz\n",f_max);
//		printf("i_Asec = %d\n",i_Asec);
//		printf("A1_sec = %f\n",q1[i_Asec].real);
//		printf("f_sec = %fHz\n",f_sec);
//		printf("A1_max = %f\n",1.0);
//		printf("A2_max = %f\n",q1[i_Amax*2].real/q1[i_Amax].real);
//		printf("A3_max = %f\n",q1[i_Amax*3].real/q1[i_Amax].real);
//		printf("A4_max = %f\n",q1[i_Amax*4].real/q1[i_Amax].real);
//		printf("A5_max = %f\n",q1[i_Amax*5].real/q1[i_Amax].real);
		
		//		printf("A1_max = %f\n",1.0);
//		printf("A2_max = %f\n",q1[i_Amax*2].real/q1[i_Amax].real);
//		printf("A3_max = %f\n",q1[i_Amax*3].real/q1[i_Amax].real);
//		printf("A4_max = %f\n",q1[i_Amax*4].real/q1[i_Amax].real);
//		printf("A5_max = %f\n",q1[i_Amax*5].real/q1[i_Amax].real);
//		float temp = q1[i_Amax*2].real*q1[i_Amax*2].real+q1[i_Amax*3].real*q1[i_Amax*3].real+q1[i_Amax*4].real*q1[i_Amax*4].real+q1[i_Amax*5].real*q1[i_Amax*5].real;
		printf("A1_max = %f\n",A_max);
		printf("A1_1 = %f\n",A_1);
		printf("A1_2 = %f\n",A_2);
		printf("A1_3 = %f\n",A_3);
		printf("A1_4 = %f\n",A_4);
		float temp =A_1*A_1+A_2*A_2+A_3*A_3+A_4*A_4;
		temp = sqrt(temp)/A_max;
		printf("THD = %f\n",temp);
}
void FFT_Init(void)
{

		for(uint32_t i=0; i<FFT_LENGTH; i++)
		{
			/* *(0.54f-0.46f*cosf(6.283185307f*i/FFT_LENGTH)) */
			q1[i].real = adc1_data[i] *(0.5-0.5*cos(6.283185307*i/FFT_LENGTH))*3.3/65536;		//数据加窗
//			q1[i].real = adc1_data[i] *3.3/65536;		
			q1[i].imag = 0;
		}
		FFT(q1, 13);
		for(uint32_t i=0; i<FFT_LENGTH; i++)
		{
			arm_sqrt_f32((float)(q1[i].real *q1[i].real+ q1[i].imag*q1[i].imag ), &q1[i].real); 
		}
	
}


unsigned char FFT(Complex *d,int m)
{
#ifndef __EXTERN_W__
	static Complex *w;
	static int mw = 0;
	float arg, w_real, w_imag, wr_real, wr_imag, wtemp;
#endif
	static int n = 1;
	Complex temp, tmp1, tmp2;
	Complex *di, *dip, *dj, *wp;
	int i, j, k, l, le, wi;
#ifndef __EXTERN_W__
	if(m != mw)
	{
		if(mw != 0)
			free(w);
		mw = m;
		if(m == 0)
			return 0;
		n = 1 << m;
		le = n >> 1;
		w =q;
		if(!w)
			return 0;
		arg = 4.0 * atan(1.0) / le;
		wr_real = w_real = cos(arg);
		wr_imag = w_imag = -sin(arg);
		dj = w;
		for(j = 1; j < le; j++)
		{
			dj->real = (float)wr_real;
			dj->imag = (float)wr_imag;
			dj++;
			wtemp = wr_real * w_real - wr_imag * w_imag;
			wr_imag = wr_real * w_imag + wr_imag * w_real;
			wr_real = wtemp;
		}
	}
#else
	n = 1 << m;
#endif
	le = n;
	wi = 1;
	for(l = 0; l < m; l++)
	{
		le >>= 1;
		for(i = 0; i < n; i += (le << 1))
		{
			di = d + i;
			dip = di + le;
			temp.real = (di->real + dip->real);
			temp.imag = (di->imag + dip->imag);
			dip->real = (di->real - dip->real);
			dip->imag = (di->imag - dip->imag);
			*di = temp;
		}
		wp = (Complex*)w + wi - 1;
		for(j = 1; j < le; j++)
		{
			tmp1 = *wp;
			for(i = j; i < n; i += (le << 1))
			{
				di = d + i;
				dip = di + le;
				temp.real = (di->real + dip->real);
				temp.imag = (di->imag + dip->imag);
				tmp2.real = (di->real - dip->real);
				tmp2.imag = (di->imag - dip->imag);
				dip->real = (tmp2.real * tmp1.real - tmp2.imag * tmp1.imag);
				dip->imag = (tmp2.real * tmp1.imag + tmp2.imag * tmp1.real);
				*di = temp;
			}
			wp += wi;
		}
		wi <<= 1;
	}

	for(i = 0; i < n; i++)
	{
		j = 0;
		for(k = 0; k < m; k++)
			j = (j << 1) | ((i >> k) & 1);
		if(i < j)
		{
			di = d + i;
			dj = d + j;
			temp = *dj;
			*dj = *di;
			*di = temp;
		}
	}
	return 1;
}

