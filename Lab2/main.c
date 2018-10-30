#include <stdio.h>
#include <sys/time.h>
#include <gsl/gsl_blas.h>

int main() {
    static double MULT = 10000000;

    struct timeval  tv1, tv2;

    FILE *file = fopen("data3.csv", "w");
    fprintf(file, "\"typ\";\"rozmiar\";\"pomiar\"\n");

    for(int i = 1000 ; i < 50000 ; i += 500) {
        gsl_vector *x = gsl_vector_alloc(i);
        gsl_vector *y = gsl_vector_alloc(i);
        printf("%i - \n", i);
        for(int k = 0 ; k < i ; k++) {
            gsl_vector_set(x, k, 100000*i);
            gsl_vector_set(y, k, 100000*i);
        }
        double *result = malloc(i);

        for(int j = 0 ; j < 10 ; j++) {
            gettimeofday(&tv1, NULL);
            gsl_blas_ddot(x, y, result);
            gettimeofday(&tv2, NULL);

            fprintf(file, "0;%i;%f\n", i, ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec)) * MULT);
        }

        gsl_vector_free(x);
        gsl_vector_free(y);
    }

    for(int i = 20 ; i < 150 ; i += 3) {
        gsl_vector *v = gsl_vector_alloc(i);
        gsl_vector *result = gsl_vector_alloc(i);
        gsl_matrix *matrix = gsl_matrix_alloc(i, i);

        for(int k = 0 ; k < i ; k++) {
            gsl_vector_set(v, k, 100000*i);
            for(int j = 0; j < i; j++)
                gsl_matrix_set(matrix, k, j, 100000*i);
        }

        for(int j = 0 ; j < 10 ; j++) {
            gettimeofday(&tv1, NULL);
            gsl_blas_dgemv(CblasNoTrans, 5.0, matrix, v, 1.0, result);
            gettimeofday(&tv2, NULL);

            fprintf(file, "1;%i;%f\n", i, ((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec)) * MULT);
        }

        gsl_vector_free(v);
        gsl_vector_free(result);
        gsl_matrix_free(matrix);
    }

    fclose(file);

    return 0;
}