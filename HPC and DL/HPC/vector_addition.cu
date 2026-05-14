#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

// CUDA Kernel
__global__ void vectorAdd(float *A, float *B, float *C, int N)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < N)
    {
        C[i] = A[i] + B[i];
    }
}

int main()
{
    int N;

    printf("Enter number of elements: ");
    scanf("%d", &N);

    int size = N * sizeof(float);

    // Host memory
    float *h_A = (float*)malloc(size);
    float *h_B = (float*)malloc(size);
    float *h_C = (float*)malloc(size);

    // Input first vector
    printf("Enter elements of first vector:\n");

    for (int i = 0; i < N; i++)
    {
        scanf("%f", &h_A[i]);
    }

    // Input second vector
    printf("Enter elements of second vector:\n");

    for (int i = 0; i < N; i++)
    {
        scanf("%f", &h_B[i]);
    }

    // Device memory
    float *d_A, *d_B, *d_C;

    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy Host to Device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    // Thread configuration
    int threadsPerBlock = 256;

    int blocksPerGrid =
    (N + threadsPerBlock - 1) / threadsPerBlock;

    // Launch Kernel
    vectorAdd<<<blocksPerGrid, threadsPerBlock>>>
    (d_A, d_B, d_C, N);

    cudaDeviceSynchronize();

    // Copy result back
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);

    // Display result
    printf("\nResultant Vector:\n");

    for (int i = 0; i < N; i++)
    {
        printf("%.2f ", h_C[i]);
    }

    printf("\n");

    // Free device memory
    cudaFree(d_A);
    cudaFree(d_B);
    cudaFree(d_C);

    // Free host memory
    free(h_A);
    free(h_B);
    free(h_C);

    return 0;
}