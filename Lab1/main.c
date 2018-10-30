#include <stdio.h>
#include <math.h>

int main(void) {

    for(double a = 0.2 ; a > 0 ; a -= 0.05) {
        int b = 10;
        int c = 1;
        double delta = pow(b, 2);
        double x1 = (-b + sqrt(delta)) / 2 * a;
        double x2 = (-b - sqrt(delta)) / 2 * a;
        printf("Invalid algorithm\n");
        printf("%f\t%f", x1, x2);

        double x3 = (c / a * 2 * a) / (-b - sqrt(delta));
        double x4 = c / (a * x3);
        printf("\nCorrect algorithm\n");
        printf("%f\t%f\n\n", x3, x4);
    }
    return 0;
}