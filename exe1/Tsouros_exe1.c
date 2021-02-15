#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

int main(int argc, char **argv) {
  double dx, dt, c, temp_M, x_ini = 0.0, x_fin = 12.0, t_ini = 0.0,
                            t_fin = 5 * M_PI, fTimeStart, fTimeEnd;
  double a = sqrt(2 / (pow(M_PI, 2.0)));
  int i, j, N = 200, num_t = 2, M, paral_flag=0;

  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'N':
        sscanf(argv[i + 1], "%d", &N);
        break;
      case 't':
        sscanf(argv[i + 1], "%d", &num_t);
        break;
      case 'p':
        paral_flag=1;
        break;
      }
    }
  }

  // Record start time
  fTimeStart = omp_get_wtime();

  dx = (x_fin - x_ini) / (N - 1);
  dt = 0.5 * (dx / a);
  c = a * (dt / dx);
  temp_M = 1 + (t_fin - t_ini) / dt;
  M = (int)round(temp_M);
  printf("dx = %f, dt = %f, c = %f, M=%d, N=%d a=%f\n", dx, dt, c, M, N, a);
  /* Initialize `u` array */
  double **u = malloc(N * sizeof(double));
  for (i = 0; i < N; i++) {
    u[i] = (double *)malloc(M * sizeof(double));
  }
  /* Initialize time and space arrays */
  double *x, *t;
  x = (double *)malloc(N * sizeof(double));
  t = (double *)malloc(M * sizeof(double));

#pragma omp parallel num_threads(num_t) firstprivate(i, j) if (paral_flag) \
    shared(x, t, u, dt, dx, N, M, c) default(none)
  {
#pragma omp for
    for (i = 0; i < N; i++) {
      x[i] = 0 + i * dx;
    }

#pragma omp for
    for (i = 0; i < M; i++) {
      t[i] = 0 + i * dt;
    }

/* Initial Conditions */
#pragma omp for
    for (i = 0; i < N; i++) {
      if (x[i] >= 2.0 && x[i] <= 4.0)
        u[i][0] = sin(M_PI * x[i]);
      else
        u[i][0] = 0;
    }
/* boundary conditions. */
#pragma omp for
    for (j = 0; j < M; j++) {
      u[0][j] = 0;
      u[N - 1][j] = 0;
    }
  }
  for (j = 0; j < M - 1; j++) {
#pragma omp parallel num_threads(num_t) firstprivate(N, j) private(i) if (paral_flag) \
    shared(u, c) default(none)
    {
#pragma omp for
      for (i = 1; i < N - 1; i++) {
        u[i][j + 1] =
            u[i][j] - (c / 2.) * (u[i + 1][j] - u[i - 1][j]) +
            (pow(c, 2) / 2.) * (u[i + 1][j] - 2 * u[i][j] + u[i - 1][j]);
      }
    }
  } 
  fTimeEnd = omp_get_wtime();
  printf("Threads Used: %d \nWall clock time:  = %.10f \n\n", num_t, (fTimeEnd - fTimeStart));
  FILE *fil;
  char filename[256];
  if(paral_flag){
    snprintf(filename, sizeof(filename), "res%d_threads.txt", num_t);}
  else snprintf(filename, sizeof(filename), "%s", "res_serial.txt");
  fil = fopen(filename, "w");
  for (i = 0; i < N; i++) {
    for (j = 0; j < M; j++) {
      fprintf(fil, " %f", u[i][j]);
    }
  }
  for (i = 0; i < N; i++)
    free(u[i]);
  free(u);
  free(x);
  free(t);
  return 0;
}
