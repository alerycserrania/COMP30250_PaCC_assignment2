#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <cblas.h>

double compute_mult_blocked_ijk_atlas(int N, int BS, double *A, double *B, double *C);
void print_blocked_matrix(int N, int BS, double* M);
void print_matrix(int N, double* M);

int main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;
    double *A, *B, *C;
    double avg_time;
    int N, BS, i;
    char PRINT_RESULT, PRINT_TIME;

    if(argc != 5)
    {
       printf("Please, use: %s N BS PRINT_RESULT PRINT_TIME:\n", argv[0]);
       printf("\t- N: matrix size\n");
       printf("\t- BS: block size\n");
       printf("\t- PRINT_RESULT (y/n): print result to stdout\n");
       printf("\t- PRINT_TIME (y/n): print time elasped to stdout\n");
       exit(EXIT_FAILURE);
    }

    if (N % BS != 0) 
    {
        printf("BS (%d) should be a factor of N (%d)", N, BS);
        exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    BS = atoi(argv[2]);
    PRINT_RESULT = argv[3][0];
    PRINT_TIME = argv[4][0];

    A = malloc(N * N * sizeof(double));
    B = malloc(N * N * sizeof(double));
    C = malloc(N * N * sizeof(double));

    srand(time(NULL));
    for (i = 0; i < N * N; i++)
    {
        A[i] = 1. + i;
        B[i] = 1. + i;
        C[i] = 0.;
    }

    gettimeofday(&tv1, &tz);
    compute_mult_blocked_ijk_atlas(N, BS, A, B, C);
    gettimeofday(&tv2, &tz);
    
    if (PRINT_RESULT == 'y')
    {
        printf("A = \n");
        print_blocked_matrix(N, BS, A);
        printf("B = \n");
        print_blocked_matrix(N, BS, B);
        printf("C = \n");
        print_blocked_matrix(N, BS, C);
    }
    
    if (PRINT_TIME == 'y')
    {
        elapsed = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec)*1e-6;
        printf("elapsed: %fs\n", elapsed);
    }
    

    free(A);
    free(B);
    free(C);

    return 0;
}

double compute_mult_blocked_ijk_atlas(int N, int BS, double *A, double *B, double *C)
{
    int i, j, k;

    for (i = 0; i < N; i+=BS) 
    {
        for (j = 0; j < N; j+=BS)
        {
            double * oC = C + (i * N + j * BS);
            for (k = 0; k < N; k+=BS)
            {
                double * oB = B + (k * N + j * BS);
                double * oA = A + (i * N + k * BS);
                ATL_dgemm(CblasNoTrans, CblasNoTrans, BS, BS, BS, 1.0, oA, BS, oB, BS, 1.0, oC, BS);
            }
        }
    }
}

void print_blocked_matrix(int N, int BS, double* M)
{
    for (int bsi = 0; bsi < N; bsi+=BS)
    {
        for (int bi = 0; bi < BS; bi++)
        {
            for (int bsj = 0; bsj < N; bsj+=BS)
            {
                for (int bj = 0; bj < BS; bj++)
                {
                    printf("%f\t ", M[bsj*BS + bj*BS + bsi*N + bi]);
                }
            }
            printf("\n");
        }
    }
}

void print_matrix(int N, double* M)
{
    int i, j, k;

    for (i = 0; i < N; i++) 
    {
        for (j = 0; j < N; j++) 
        {
            printf("%f\t", M[i * N + j]);
        }
        printf("\n");
    }
}