#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

double compute_mult_ijk(int N, double *A, double *B, double *C);
void print_matrix(int N, double* M);

int main(int argc, char *argv[])
{
    struct timeval tv1, tv2;
    struct timezone tz;
    double elapsed;
    double *A, *B, *C;
    double avg_time;
    int N, i;
    char PRINT_RESULT, PRINT_TIME;

    if(argc != 4)
    {
       printf("Please, use: %s N PRINT_RESULT PRINT_TIME:\n", argv[0]);
       printf("\t- N: matrix size\n");
       printf("\t- PRINT_RESULT (y/n): print result to stdout\n");
       printf("\t- PRINT_TIME (y/n): print time elasped to stdout\n");
       exit(EXIT_FAILURE);
    }

    N = atoi(argv[1]);
    PRINT_RESULT = argv[2][0];
    PRINT_TIME = argv[3][0];

    A = malloc(N * N * sizeof(double));
    B = malloc(N * N * sizeof(double));
    C = malloc(N * N * sizeof(double));

    srand(time(NULL));

    for (i = 0; i < N * N; i++)
    {
        A[i] = rand() % 10 - 5;
        B[i] = rand() % 10 - 5;
        C[i] = 0.;
    }

    gettimeofday(&tv1, &tz);
    compute_mult_ijk(N, A, B, C);
    gettimeofday(&tv2, &tz);
    
    if (PRINT_RESULT == 'y')
    {
        printf("A = \n");
        print_matrix(N, A);
        printf("B = \n");
        print_matrix(N, B);
        printf("C = \n");
        print_matrix(N, C);
    }

    if (PRINT_TIME == 'y')
    {
        elapsed = (double) (tv2.tv_sec-tv1.tv_sec) + (double) (tv2.tv_usec-tv1.tv_usec)*1e-6;
        printf("elapsed: %fs\n", elapsed);
    }

    

    return 0;
}

double compute_mult_ijk(int N, double *A, double *B, double *C)
{
    int i, j, k;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            for (k = 0; k < N; k++)
            {
                C[i * N + j] += A[i * N + k] * B[k * N + j];
            }
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