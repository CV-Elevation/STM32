#include "fft.h"
#include "arm_math.h"
#include "stdio.h"

#define FFT_NUM 1024

arm_rfft_fast_instance_f32 u_fft_s;
float fft_buffer[FFT_NUM];
float amplitude_specturm_temp[FFT_NUM / 2];
float amplitude_specturm[FFT_NUM / 2];

void arm_fft_init(void)
{
	arm_rfft_fast_init_f32(&u_fft_s, FFT_NUM);
}

void arm_fft_demo(float *src)
{
	arm_rfft_fast_f32(&u_fft_s, src, fft_buffer, 0);
	arm_cmplx_mag_squared_f32(fft_buffer, amplitude_specturm_temp, FFT_NUM / 2);
	for (int i = 0; i < FFT_NUM / 2; i++)
	{
		arm_sqrt_f32(amplitude_specturm_temp[i], &amplitude_specturm[i]);
		if (i == 0)
		{
			amplitude_specturm[i] /= (float)FFT_NUM;
		}
		else
		{
			amplitude_specturm[i] /= (float)(FFT_NUM / 2);
		}
	}
	float freq;
	for (int i = 0; i < FFT_NUM / 2; i++)
	{
		freq = i / (float)FFT_NUM * 1000.0f;
		printf("%.2f,%.2f\n", freq, amplitude_specturm[i]);
	}
}