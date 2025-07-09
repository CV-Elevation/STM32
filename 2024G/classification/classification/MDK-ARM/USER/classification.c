#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "fc1.h"
#include "fc2.h"
#include "main.h"
#include "u_fft.h"
#include "classification.h"
#include "float.h"
#include "adc.h"
// Define constants
#define INPUT_SIZE 511
#define HIDDEN_SIZE 32
#define OUTPUT_SIZE 2

// ReLU activation function
void relu(float *input, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (input[i] < 0)
        {
            input[i] = 0;
        }
    }
}

// Softmax function
void softmax(float *input, int size)
{
    float max = input[0];
    for (int i = 1; i < size; i++)
    {
        if (input[i] > max)
        {
            max = input[i];
        }
    }

    float sum = 0;
    for (int i = 0; i < size; i++)
    {
        input[i] = exp(input[i] - max);
        sum += input[i];
    }

    for (int i = 0; i < size; i++)
    {
        input[i] /= sum;
    }
}

// Forward pass through the network
int forward(float *input)
{
    // Hidden layer
    float output[2];
    float hidden[HIDDEN_SIZE] = {0};
    for (int i = 0; i < HIDDEN_SIZE; i++)
    {
        for (int j = 0; j < INPUT_SIZE; j++)
        {
            hidden[i] += fc1_weights[i * INPUT_SIZE + j] * input[j];
        }
        hidden[i] += fc1_bias[i];
    }
    relu(hidden, HIDDEN_SIZE);

    // Output layer
    for (int i = 0; i < OUTPUT_SIZE; i++)
    {
        output[i] = 0;
        for (int j = 0; j < HIDDEN_SIZE; j++)
        {
            output[i] += fc2_weights[i * HIDDEN_SIZE + j] * hidden[j];
        }
        output[i] += fc2_bias[i];
    }
    softmax(output, OUTPUT_SIZE);
    printf("%.4f,%.4f\r\n", output[0], output[1]);
    return output[1] > output[0] ? 1 : 0;
}

int classification()
{
    for (int i = 0; i < 512; i++)
    {
        pAmp[i] = 0;
    }
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)raw_voltage_buffer, SIMPLE_LEN);
    adc_flag = 0;
    while (!adc_flag)
        ;
    adc_flag = 0;
    for (int i = 0; i < FFT_NUM; i++)
    {
        for (int j = 0; j < FFT_LEN; j++)
        {
            voltage[j] = (float)raw_voltage_buffer[j + i * FFT_LEN];
        }
        u_fft_demo(voltage);
    }
HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
int result = forward(&pAmp[1]);
return result;
}
