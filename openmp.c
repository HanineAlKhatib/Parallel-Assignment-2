#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define M 1000
#define N 1000
#define P 1000
#define NUM_THREADS 16

int A[M][N];
int B[N][P];
int C[M][P];

int main() {
   // Initialize matrices
   for (int i = 0; i < M; i++) {
      for (int j = 0; j < N; j++) {
A[i][j] = rand() % 10;
}
}

for (int i = 0; i < N; i++) {
for (int j = 0; j < P; j++) {
B[i][j] = rand() % 10;
}
}

// Multiply matrices in parallel
#pragma omp parallel num_threads(NUM_THREADS)
{
int tid = omp_get_thread_num();
int start = tid * M / NUM_THREADS;
int end = (tid + 1) * M / NUM_THREADS;


  for (int i = start; i < end; i++) {
     for (int j = 0; j < P; j++) {
        C[i][j] = 0;
        for (int k = 0; k < N; k++) {
           C[i][j] += A[i][k] * B[k][j];
        }
     }
  }
}

// Print result matrix
for (int i = 0; i < M; i++) {
for (int j = 0; j < P; j++) {
printf("%d ", C[i][j]);
}
printf("\n");
}

return 0;
}