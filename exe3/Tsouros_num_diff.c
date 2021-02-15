#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

double func(double x) { return x * sin(x) + exp(x); }

double forward(double *val, double step, int i) {
  return (-func(val[i + 2]) + 4 * func(val[i + 1]) - 3 * func(val[i])) /
         (2 * step);
}

double backward(double *val, double step, int i) {
  return (3 * func(val[i]) - 4 * func(val[i - 1]) + func(val[i - 2])) /
         (2 * step);
}

double central(double *val, double step, int i) {
  return (func(val[i + 1]) - func(val[i - 1])) / (2 * step);
}

int main(int argc, char **argv) {
  int i, N, num_t = 1, paral_flag = 0;
  double x_init, x_fin;
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'N':
        sscanf(argv[i + 1], "%d", &N);
        break;
      case 's':
        sscanf(argv[i + 1], "%lf", &x_init);
        break;
      case 'f':
        sscanf(argv[i + 1], "%lf", &x_fin);
        break;
      case 't':
        sscanf(argv[i + 1], "%d", &num_t);
        break;
      case 'p':
        paral_flag = 1;
        break;
      }
    }
  }
  double *x = (double *)malloc((N + 1) * sizeof(double));
  double *res = (double *)malloc((N + 1) * sizeof(double));
  double step = (x_fin - x_init) / N;

  // Record start time
  double fTimeStart = omp_get_wtime();

#pragma omp parallel num_threads(num_t) shared(x) private(i)                   \
    firstprivate(x_init, step, N) default(none) if (paral_flag)
  {
#pragma omp for
    for (i = 0; i <= N; i++) {
      x[i] = x_init + i * step;
    }
  }

#pragma omp parallel num_threads(num_t) shared(res) private(i) \
  firstprivate(step, N, x) default(none) if (paral_flag)
  {
#pragma omp for
    for (i = 0; i <= N; i++) {
      if (i == 0)
        res[i] = forward(x, step, i);
      else if (i < N)
        res[i] = central(x, step, i);
      else
        res[i] = backward(x, step, i);
    }
  }
  // Record end time
  double fTimeEnd = omp_get_wtime();

  // I/O operations
  printf("Threads Used: %d \nWall clock time:  = %.10f \n\n", num_t,
         (fTimeEnd - fTimeStart));
  FILE *fil;
  char filename[256];
  if (paral_flag) {
    snprintf(filename, sizeof(filename), "res%d_threads.txt", num_t);
  } else
    snprintf(filename, sizeof(filename), "%s", "res_serial.txt");
  fil = fopen(filename, "w");
  for (i = 0; i <= N; i++) {
    fprintf(fil, " %f", res[i]);
  }

  free(x);
  free(res);
}
