#include <stdio.h>
#include <sys/time.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_complex_math.h>

void naive(double **A, double **B, double **C, int size) {
    for(int i = 0 ; i < size ; i++) {
        for(int j = 0 ; j < size ; j++) {
            for(int k = 0 ; k < size ; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            };
        }
    }
}

void better(double **A, double **B, double **C, int size) {
    for(int i = 0 ; i < size ; i++) {
        for(int j = 0 ; j < size ; j++) {
            for(int k = 0 ; k < size ; k++) {
                C[i][k] = C[i][k] + A[i][j] * B[j][k];
            };
        }
    }
}

int main() {
    struct timeval  tv1, tv2;

    FILE *file = fopen("data.csv", "w");
    fprintf(file, "\"size\";\"naive\";\"better\";\"blas\"\n");

    for(int i = 10 ; i < 500 ; i += 10) {
        gsl_matrix_complex *matrix1 = gsl_matrix_complex_alloc(i, i);
        gsl_matrix_complex *matrix2 = gsl_matrix_complex_alloc(i, i);
        gsl_matrix_complex *result = gsl_matrix_complex_alloc(i, i);

        for(int k = 0 ; k < i ; k++) {
            for(int j = 0; j < i; j++) {
                gsl_matrix_complex_set(matrix1, k, j, gsl_complex_rect(1.0, 1.0));
                gsl_matrix_complex_set(matrix2, k, j, gsl_complex_rect(1.0, 1.0));
            }
        }

        for(int j = 0 ; j < 10 ; j++) {
            double **A = malloc( i * sizeof(double*));
            for (int v = 0; v < i; v++)
            {
                A[v] = malloc(i * sizeof(double));
            }
            double **B = malloc( i * sizeof(double*));
            for (int v = 0; v < i; v++)
            {
                B[v] = malloc(i * sizeof(double));
            }
            double **C = malloc( i * sizeof(double*));
            for (int v = 0; v < i; v++)
            {
                C[v] = malloc(i * sizeof(double));
            }

            gettimeofday(&tv1, NULL);
            naive(A, B, C, i);
            gettimeofday(&tv2, NULL);

            free(A);
            free(B);

            fprintf(file, "%i;%f;", i, ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec)));

            A = malloc( i * sizeof(double*));
            for (int v = 0; v < i; v++)
            {
                A[v] = malloc(i * sizeof(double));
            }
            B = malloc( i * sizeof(double*));
            for (int v = 0; v < i; v++)
            {
                B[v] = malloc(i * sizeof(double));
            }
            C = malloc( i * sizeof(double*));
            for (int v = 0; v < i; v++)
            {
                C[v] = malloc(i * sizeof(double));
            }

            gettimeofday(&tv1, NULL);
            better(A, B, C, i);
            gettimeofday(&tv2, NULL);

            free(A);
            free(B);

            fprintf(file, "%f;", ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec)));

            gettimeofday(&tv1, NULL);
            gsl_blas_zgemm(CblasNoTrans, CblasNoTrans, gsl_complex_rect(1.0, 1.0), matrix1, matrix2, gsl_complex_rect(1.0, 1.0), result);
            gettimeofday(&tv2, NULL);


            fprintf(file, "%f\n", ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec)));
        }
        printf("num %i\n", i);

        gsl_matrix_complex_free(matrix1);
        gsl_matrix_complex_free(matrix2);
        gsl_matrix_complex_free(result);
    }

    fclose(file);

    return 0;
}