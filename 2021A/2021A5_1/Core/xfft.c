#include "xFFT.h"
#include "math.h"

float Fs;
float f[5];
float temp;
float U[5];
ALIGN_32BYTES (Complex q[FFT_LENGTH])   __attribute__((section(".ARM.__at_0x24000000")) );
ALIGN_32BYTES (Complex q1[FFT_LENGTH])  __attribute__((section(".ARM.__at_0x24040000")) );

void FFT_DIS(void)
{	
	//打印频谱图
//		for(uint32_t i=1550; i<6750; i++)
//		for(uint32_t i = 0; i < FFT_LENGTH; i++) 
//		for(uint32_t i=0; i<FFT_LENGTH/2; i++)
//		{
//			printf("%f\r\n", q1[i].real);
////			printf("%d:\t%f\r\n", i,  q1[i].real);			
//		}
	
	//求THD	
		float A_max =0;
		float A_1 =0;
		float A_2 =0;
		float A_3 =0;
		float A_4 =0;
		uint32_t i_A[5];
		uint32_t i =0;
		for(i=1;i<=FFT_LENGTH/2;i++)
		{
			if(q1[i].real >= A_max&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_max = q1[i].real;
				i_A[0] = i;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_1)&&(q1[i].real<A_max)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_1 = q1[i].real;
				i_A[1] = i;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_2)&&(q1[i].real<A_1)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_2 = q1[i].real;
				i_A[2] = i;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_3)&&(q1[i].real<A_2)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_3 = q1[i].real;
				i_A[3] = i;
			}
		}
		for(i=1;i<FFT_LENGTH/2;i++)
		{

			if((q1[i].real >A_4)&&(q1[i].real<A_3)&&q1[i].real>q1[i+1].real&&q1[i].real>q1[i-1].real)
			{
				A_4 = q1[i].real;
				i_A[4] = i;
			}
		}
		float A[5];
		A[0]=A_max;
		A[1]=A_1;
		A[2]=A_2;
		A[3]=A_3;
		A[4]=A_4;

		//计算频率
		if(Fs_Flag)
		{
			Fs = 240000000 / 100 / 80;					// 采样率	
		}
		else
		{
			Fs = 240000000 / 1 / 80;					// 采样率				
		}
		printf("%f\n",Fs);
		for(u8 i=0;i<=4;i++)
		{
		 f[i] = i_A[i] * Fs / FFT_LENGTH;
		}
		//归一化幅值
		float gy[5];
		for(u8 i=0;i<=4;i++)
		{
		 gy[i]=f[i]/f[0];
		}
		u8 judge[5]={0};//记录哪几次谐波存在
		u8 store[5];
		for(u8 i=0;i<=4;i++)
		{
			for(u8 j=0;j<=4;j++)
			{
				if(gy[j]<=i+1.4&&gy[j]>=i+0.6)
				{
					judge[i]=1;
					store[i]=j;
				}
			}
		}
		for(u8 i=0;i<=4;i++)
		{
			if(judge[i]==0)U[i]=0;
			else U[i]=A[store[i]]/A[0];
//		printf("%d\n",store[i]);
		}
		
		printf("base_fre = %fHz\n",f[0]);
		printf("1_fre = %fHz\n",f[1]);
		printf("2_fre = %fHz\n",f[2]);
		printf("3_fre = %fHz\n",f[3]);
		printf("4_fre = %fHz\n",f[4]);

		printf("A1_max = %f\n",A_max);
		printf("A1_1 = %f\n",A_1);
		printf("A1_2 = %f\n",A_2);
		printf("A1_3 = %f\n",A_3);
		printf("A1_4 = %f\n",A_4);
		for(u8 i=0;i<=4;i++)
	{
		printf("%f\n",U[i]);

	}
		temp =A_1*A_1+A_2*A_2+A_3*A_3+A_4*A_4;
		temp = sqrt(temp)/A_max*100;
//		temp = U[1]*U[1]+U[2]*U[2]+U[3]*U[3]+U[4]*U[4];
//		temp = sqrt(temp)/U[0]*100;

//		printf("THD = %f\n",temp);
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

