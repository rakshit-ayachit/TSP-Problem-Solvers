#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <mpi.h>

#define V 10

int min(int a, int b) {
    return (a < b) ? a : b;
}

int tsp(int graph[][V], int SetOfVisitedCities, int CurrentCity, int TotalCities, int dp[][V]) {
    if (SetOfVisitedCities == (1 << TotalCities) - 1) {
        return graph[CurrentCity][0];
    }
    
    if (dp[SetOfVisitedCities][CurrentCity] != -1) {
        return dp[SetOfVisitedCities][CurrentCity];
    }
    int ans = INT_MAX;

    for (int city = 0; city < TotalCities; city++) {
        if ((SetOfVisitedCities & (1 << city)) == 0) {
            int newAns = graph[CurrentCity][city] + tsp(graph, SetOfVisitedCities | (1 << city), city, TotalCities, dp);
            ans = min(ans, newAns);
        }
    }
    return dp[SetOfVisitedCities][CurrentCity] = ans;
}

int main(int argc, char** argv) {
    int rank, size;
    double start_time, end_time;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    if (size != V) {
        if (rank == 0) {
            printf("Number of processes should be equal to the number of vertices\TotalCities");
        }
        MPI_Finalize();
        return 1;
    }

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

    int dp[1 << V][V];
    for (int i = 0; i < (1 << V); i++) {
        for (int j = 0; j < V; j++) {
            dp[i][j] = -1;
        }
    }


    int local_ans = tsp(graph, 1 << rank, rank, V, dp);
    int global_ans;
    MPI_Reduce(&local_ans, &global_ans, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (rank == 0) {
        printf("This is the Parallelized Approach with MPI: \n");
        printf("Minimum cost: %d\n", global_ans);
        printf("Time taken: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}


