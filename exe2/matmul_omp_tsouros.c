/****************************************
 ***
 *** SQUARE Matrix Multiplication (serial)
 ***
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ROWS 512
#define COLUMNS 512

/* Prototype */
double ** array2D (int nRows, int nColumns);

int main(int argc, char **argv) {

  int i, j, k, paral_flag=0, num_t=1, small_mat=0;
  /* double sum; */
  double **a, **b, **c;

  /* Allocate Arrays */
  a = array2D(ROWS, COLUMNS);
  b = array2D(ROWS, COLUMNS);
  c = array2D(ROWS, COLUMNS);

  
  /* Argument Parsing for pallelization & number of threads to use */
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {
      case 'p':
        paral_flag = 1;
        break;
      case 't':
        sscanf(argv[i + 1], "%d", &num_t);
        break;
      }
    }
  }

  /* Force serial execution for small matrices. Parallelization below
this limit results in increase in execution time. Also set small matrix flag
for writing to file */
  if (ROWS < 32 || COLUMNS < 32) {
    paral_flag = 0;
    small_mat = 1;
  }

  /* Initialize */

  for (i=0; i<ROWS; i++) {
    for (j=0; j<COLUMNS; j++) {
      a[i][j] = 3.0;
      b[i][j] = 2.0;
      c[i][j] = 0.0;
    }
  }

  if (ROWS < 10) 
  {
    /* TEST PRINT */  
    for (i=0; i<ROWS; i++) {
      for (j=0; j<COLUMNS; j++) {
        printf("%f ", a[i][j]);
      }
      printf("\n");
    }

    for (i=0; i<ROWS; i++) {
      for (j=0; j<COLUMNS; j++) {
        printf("%f ", b[i][j]);
      }
      printf("\n");
    }

    for (i=0; i<ROWS; i++) {
      for (j=0; j<COLUMNS; j++) {
        printf("%f ", c[i][j]);
      }
      printf("\n");
    }
    printf("\n");
  }

  /* Start timing */
  double start_time = omp_get_wtime(); 

  /* 
    Multiply Matrices 
    (SQUARE)
  */
#pragma omp parallel num_threads(num_t) shared(a,b,c) private(i,j,k)  if (paral_flag)
  {
#pragma omp for 
  for (i=0; i<ROWS; i++) {
    for (j=0; j<COLUMNS; j++) {
      for (k=0; k<COLUMNS; k++) {
        c[i][j] += a[i][k]*b[k][j];
      }
    }
  }
  }

  /* Stop Timing */
  double end_time = omp_get_wtime();

  /* Report Time */
  if (paral_flag){
    printf("[Matmul] Num Threads %d \nTime: %f seconds\n\n", num_t, (end_time - start_time));
  } else {
    printf("[Matmul] Serial Execution \nTime: %f seconds\n\n", (end_time - start_time));
  }

  /* PRINT RESULT */
  if (ROWS < 10)
  {
    for (i=0; i<ROWS; i++) {
      for (j=0; j<COLUMNS; j++) {
        printf("%f ", c[i][j]);
      }
      printf("\n");
    }
  }

  /* Write to file */
  FILE *fil;
  char filename[256];
  if (paral_flag) {
    snprintf(filename, sizeof(filename), "res%d_threads.txt", num_t);
  }
  else if (paral_flag==0 && small_mat) {
    snprintf(filename, sizeof(filename), "res%d_threads.txt", num_t);
  }
  else {
    snprintf(filename, sizeof(filename), "%s", "res_serial.txt");
  }
  fil = fopen(filename, "w");
  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLUMNS; j++) {
      fprintf(fil, " %f", c[i][j]);
    }
  }


  /* Free Memory (Arrays) */
  free(a[0]); 
  free(a);
  free(b[0]); 
  free(b);
  free(c[0]); 
  free(c);
  return 0;
}


/* 
  Dynamically Allocates 2D Array  (Contiguous in Memory) 
*/
double ** array2D (int nRows, int nColumns) {
  
  int i;
  double ** array;

  /* Create rows (equal to gridPoints) */
  array = (double**)malloc(nRows*sizeof(double*));
  if (array == NULL) {
    printf("\n\n ERROR: Out of memory for output array! Exiting...\n\n");
    exit(-1);
  }

  /* Allocate enough memory for whole 2D array */
  array[0] = (double*)malloc(nRows*nColumns*sizeof(double));
  if (array[0] == NULL) {
    printf("\n\n ERROR: Out of memory for output array! Exiting...\n\n");
    exit(-1);
  }
  
  /* Point to individual rows */
  for (i=1;i<nRows;i++) {
    array[i] = array[0] + i*nColumns; 
  }

  return array;
}
