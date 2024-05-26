#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <omp.h>

#define V 10


int min(int a, int b) {
    return (a < b) ? a : b;
}

int tsp(int graph[][V], int SetOfVisitedCities, int CurrentCity, int TotalCities, int **dp) {
    
	if (SetOfVisitedCities == (1 << TotalCities) - 1) {
        return graph[CurrentCity][0];
    }

    if (dp[SetOfVisitedCities][CurrentCity] != -1) {
        return dp[SetOfVisitedCities][CurrentCity];
    }
    
	int ans = INT_MAX;
	
    #pragma omp parallel for reduction(min:ans) schedule(dynamic, 1)
    for (int city = 0; city < TotalCities; city++) {
        if ((SetOfVisitedCities & (1 << city)) == 0) {
            int newAns = graph[CurrentCity][city] + tsp(graph, SetOfVisitedCities | (1 << city), city, TotalCities, dp);
            ans = min(ans, newAns);
        }
    }
    return dp[SetOfVisitedCities][CurrentCity] = ans;
}

int main() {
    int graph[V][V];
    srand(time(NULL));
    printf("Random input matrix:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = rand() % 100;
            printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
    
	printf("\n");

    int **dp = (int **)malloc((1 << V) * sizeof(int *));
    for (int i = 0; i < (1 << V); i++) {
        dp[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            dp[i][j] = -1;
        }
    }

    struct timespec start, end;
    double cpu_time_used;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int ans = tsp(graph, 1, 0, V, dp);
    clock_gettime(CLOCK_MONOTONIC, &end);
    cpu_time_used = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("This is the Parallelized Approach with OPENMP: \n");
	printf("Minimum cost: %d\n", ans);
    printf("Time taken: %f seconds\n", cpu_time_used);

    for (int i = 0; i < (1 << V); i++) {
        free(dp[i]);
    }
    free(dp);

    return 0;
}

