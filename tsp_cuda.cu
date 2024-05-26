#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

#define V 10

__global__ void tsp(int* graph, int n, int* dp) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= (1 << n)) return;

    for (int j = 0; j < n; j++) {
        if (i & (1 << j)) {
            int prev_i = i & ~(1 << j);
            int min_cost = INT_MAX;
            for (int k = 0; k < n; k++) {
                if (graph[k * n + j] != INT_MAX && (i == (1 << j) || (prev_i & (1 << k)))) {
                    int cost = dp[prev_i * n + k] + graph[k * n + j];
                    if (cost < min_cost) min_cost = cost;
                }
            }
            if (min_cost != INT_MAX) {
                dp[i * n + j] = min_cost;
            }
        }
    }
}

int main() {
    int graph[V][V];
    srand(time(NULL));
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = rand() % 100;
        }
    }
    
    int* d_graph;
    cudaMalloc((void**)&d_graph, V * V * sizeof(int));
    cudaMemcpy(d_graph, graph, V * V * sizeof(int), cudaMemcpyHostToDevice);

    int dp_size = (1 << V) * V * sizeof(int);
    int* d_dp;
    cudaMalloc((void**)&d_dp, dp_size);
    cudaMemset(d_dp, 0x7F, dp_size);

    for (int i = 0; i < V; i++) {
        int start_state = (1 << i) * V + i;
        cudaMemset(&d_dp[start_state], 0, sizeof(int));
    }

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    tsp << <(1 << V) / 256 + 1, 256 >> > (d_graph, V, d_dp);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float milliseconds = 0;
    cudaEventElapsedTime(&milliseconds, start, stop);

    int* dp = (int*)malloc(dp_size);
    cudaMemcpy(dp, d_dp, dp_size, cudaMemcpyDeviceToHost);

    int min_route_cost = INT_MAX;
    int final_mask = (1 << V) - 1;
    for (int i = 0; i < V; i++) {
        if (dp[final_mask * V + i] < min_route_cost) {
            min_route_cost = dp[final_mask * V + i];
        }
    }
    
	 printf("Random input matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
	printf("This is the Parallelized Approach with CUDA: \n");
    printf("Minimum cost: %d\n", min_route_cost);
    printf("Time taken: %f seconds\n", milliseconds / 1000);

    cudaFree(d_graph);
    cudaFree(d_dp);
    free(dp);

    return 0;
}

