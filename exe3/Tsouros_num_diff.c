#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double func(double x){
  return x*sin(x) + exp(x);
}

double forward(double *val, double step, int i){
  return (
          - func(val[i+2])
          + 4 * func(val[i+1])
          -3 * func(val[i])
          ) / (2 * step);
}

double backward(double *val, double step, int i){
  return (
          3 * func(val[i])
          - 4 * func(val[i-1])
          + func(val[i-2])
          ) / (2 * step);
}

double central(double *val, double step, int i){
  return (
          func(val[i+1]) - func(val[i-1])
          ) / (2 * step) ;
}

int main(int argc, char **argv) {
  int i;
  double x_init, x_fin, N;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'N':
        sscanf(argv[i + 1], "%lf", &N);
        break;
      case 's':
        sscanf(argv[i + 1], "%lf", &x_init);
        break;
      case 'f':
        sscanf(argv[i + 1], "%lf", &x_fin);
        break;
      }
    }
  }
  double* x = (double*) malloc((N + 1) * sizeof(double));
  double* res = (double*) malloc((N + 1) * sizeof(double));
  double step = (x_fin - x_init) / N;
  printf("\n STEP: %f \n", step);

  for (i = 0; i <= N; i++){
    x[i] = x_init + i * step;
  }
  for (i =0; i <= N; i++){
    if (i==0) res[i] = forward(x, step, i);
    else if (i < N) res[i] = central(x, step, i);
    else res[i] = backward(x, step, i);
  }
  free(x);
  free(res);
}
