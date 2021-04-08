#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PI 3.14159265358979323846

#define BY_ROW(row,col,len) ((row)*(len) + (col))
#define BY_COL(row,col,len) ((row) + (col)*(len))

/* Time in seconds from some point in the past */
double dwalltime();

double randFP(double min, double max) {
  double range = (max - min);
  double div = RAND_MAX / range;
  return min + (rand() / div);
}

/* Multiply (block)submatrices */
void blkmul(double *ablk, double *bblk, double *cblk, int N, int BS)
{
  int i, j, k;

  for (i = 0; i < BS; i++) {
    for (j = 0; j < BS; j++) {
      for  (k = 0; k < BS; k++) {
        cblk[BY_ROW(i,j,N)] += ablk[BY_ROW(i,k,N)] * bblk[BY_COL(k,j,N)];
      }
    }
  }
}

/* Multiply square matrices, blocked version */
void matmul(double *ma, double *mb, double *mc, int N, int BS)
{
  int i, j, k;    /* Guess what... */

  for (int i = 0; i < N; i += BS) {
    for (int j = 0; j < N; j += BS) {
      mc[BY_ROW(i,j,N)] = 0;
      for (int k = 0; k < N; k += BS) {
        blkmul(&ma[BY_ROW(i,k,N)], &mb[BY_COL(k,j,N)], &mc[BY_ROW(i,j,N)], N, BS);
      }
    }
  }
}

int main(int argc, char* argv[]){
  int N, BS;

  // Controla los argumentos al programa
  if (argc != 3 || (N = atoi(argv[1])) <= 0 || (BS = atoi(argv[2])) <= 0 || (N % BS != 0)) {
    printf("\nError, modo de uso: %s N BS (N debe ser multiplo de BS)\n", argv[0]);
    return 0;
  }

  // Reservar memoria para las matrices y auxiliares
  double *A = (double *) malloc(sizeof(double) * N * N);
  double *B = (double *) malloc(sizeof(double) * N * N);
  double *C = (double *) malloc(sizeof(double) * N * N);
  double *T = (double *) malloc(sizeof(double) * N * N);
  double *R = (double *) malloc(sizeof(double) * N * N);
  double *M = (double *) malloc(sizeof(double) * N * N);
  double *ra = (double *) malloc(sizeof(double) * N * N);
  double *rb = (double *) malloc(sizeof(double) * N * N);

  time_t t;
  srand((unsigned) time(&t));

  // Inicializar matrices A, B, T
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[BY_COL(i,j,N)] = randFP(0, 1);
      B[BY_COL(i,j,N)] = randFP(0, 1);
      T[BY_ROW(i,j,N)] = randFP(0, 1);
    }
  }

  // La matriz M tiene rango 0 a 2*PI
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      M[BY_ROW(i,j,N)] = randFP(0, 2*PI);
    }
  }

  double timetick = dwalltime();

  // Calcular AVG y 𝑅(𝑖,𝑗) = (1 − 𝑇(𝑖,𝑗))(1 − 𝑐𝑜𝑠𝜃(𝑖,𝑗)) + 𝑇(𝑖,𝑗) 𝑠𝑖𝑛𝜃(𝑖,𝑗)
  double avgR = 0;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      R[BY_ROW(i,j,N)] = (1 - T[BY_ROW(i,j,N)]) * (1 - cos(M[BY_ROW(i,j,N)]))
          + T[BY_ROW(i,j,N)] * sin(M[BY_ROW(i,j,N)]);
      avgR =+ R[BY_ROW(i,j,N)];
    }
  }
  avgR /= N*N;

  // Multiplicación R*A
  matmul(R, A, ra, N, BS);

  // Multiplicación R*B
  matmul(R, B, rb, N, BS);

  // Calcular C = T + avgR*(RA + RB)
  for(int i = 0; i < N; i++) {
    for(int j = 0; j < N; j++) {
      C[BY_ROW(i,j,N)] = 
        T[BY_ROW(i,j,N)] + avgR * (ra[BY_ROW(i,j,N)] + rb[BY_ROW(i,j,N)]);
    }
  }

  printf("Tiempo en segundos %f \n", dwalltime() - timetick);

  // Limpieza
  free(A);
  free(B);
  free(C);
  free(T);
  free(R);
  free(M);
  free(ra);
  free(rb);
}

#include <sys/time.h>

double dwalltime()
{
  double sec;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}