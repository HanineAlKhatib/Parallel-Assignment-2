#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define M 1000
#define N 1000
#define P 1000
#define NUM_THREADS 8

int A[M][N];
int B[N][P];
int C[M][P];

struct thread_data {
   int tid;
   int start;
   int end;
};

void *multiply(void *arg) {
   struct thread_data *data = (struct thread_data *) arg;
   int tid = data->tid;
   int start = data->start;
   int end = data->end;

   for (int i = start; i < end; i++) {
      for (int j = 0; j < P; j++) {
         C[i][j] = 0;
         for (int k = 0; k < N; k++) {
            C[i][j] += A[i][k] * B[k][j];
         }
      }
   }

   pthread_exit(NULL);
}

int main() {
   pthread_t threads[NUM_THREADS];
   struct thread_data thread_data_array[NUM_THREADS];
   int rc;

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
   for (int t = 0; t < NUM_THREADS; t++) {
      thread_data_array[t].tid = t;
      thread_data_array[t].start = t * M / NUM_THREADS;
      thread_data_array[t].end = (t + 1) * M / NUM_THREADS;

      rc = pthread_create(&threads[t], NULL, multiply, (void *) &thread_data_array[t]);
      if (rc) {
         printf("ERROR: return code from pthread_create() is %d\n", rc);
         exit(-1);
      }
   }

   // Wait for threads to complete
   for (int t = 0; t < NUM_THREADS; t++) {
      rc = pthread_join(threads[t], NULL);
      if (rc) {
         printf("ERROR: return code from pthread_join() is %d\n", rc);
         exit(-1);
      }
   }

   // Print result matrix
   for (int i = 0; i < M; i++) {
      for (int j = 0; j < P; j++) {
         printf("%d ", C[i][j]);
      }
      printf("\n");
   }

   pthread_exit(NULL);
}
