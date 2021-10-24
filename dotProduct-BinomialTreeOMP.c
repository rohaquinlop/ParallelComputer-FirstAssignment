#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <mpi.h>

#define ERROR_CODE    -1

#define ROOT_PROCESS  0

#define N_TAG         0
#define A_TAG         1
#define B_TAG         2
#define DOT_TAG       3

int potencia(int a, int b) {
  int ans = 1;

  while(b > 0) {
    ans *= a;
    b--;
  }
  return ans;
}

int logaritmo2(int x) {
  int ans = 0;
  //Para proposito del problema el logaritmo de cero va a ser cero.
  if(x == 0) {
    ans = 0;
  }
  //De lo contrario va a calcular el logaritmo usando corrimiento de bits.
  else {
    x >>= 1;
    while(x > 0) {
      x >>= 1;
      ans++;
    }
  }

  return ans;
}

int logaritmoAux(int x){
  int ans = 0;

  while(x > 0)
  {
    x >>= 1;
    ans++;
  }

  return ans;
}

int main(int argc, char *argv[]){
  int* A;
  int* B;
  int i, n, my_rank, p;
  int dotProduct, localDotProduct, rank_dot;
  int lwr_lim, upr_lim;

  MPI_Init ( &argc, &argv );
  MPI_Comm_rank ( MPI_COMM_WORLD, &my_rank );
  MPI_Comm_size ( MPI_COMM_WORLD, &p );

  if ( p < 2 )
    /* Terminate (in a non-gracefully manner) the runtime environment */
    MPI_Abort ( MPI_COMM_WORLD, ERROR_CODE );

  for(;;){

    if ( my_rank == ROOT_PROCESS ){
      printf ("Enter the vector size: (0 quits) ");
      fflush ( stdout );
      scanf ("%d", &n );

      A = (int*) malloc(n*sizeof(int));
      B = (int*) malloc(n*sizeof(int));

      printf ("Enter the first vector values\n");
      fflush ( stdout );
      for(i = 0; i < n; i++){
        scanf ("%d", &A[i] );
        fflush ( stdout );
      }

      printf ("Enter the second vector values\n");
      fflush ( stdout );
      for(i = 0; i < n; i++){
        scanf ("%d", &B[i] );
        fflush ( stdout );
      }

      for(i = 0; i < logaritmo2(p); i++) {
        MPI_Send ( &n, 1, MPI_INT, my_rank + potencia(2 , i), N_TAG, MPI_COMM_WORLD );
        MPI_Send ( A, n, MPI_INT, my_rank + potencia(2 , i), A_TAG, MPI_COMM_WORLD );
        MPI_Send ( B, n, MPI_INT, my_rank + potencia(2 , i), B_TAG, MPI_COMM_WORLD );
      }
    }
    else{
      MPI_Recv ( &n, 1, MPI_INT, my_rank - potencia(2, logaritmo2(my_rank)), N_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
      A = (int*) malloc(n*sizeof(int));
      B = (int*) malloc(n*sizeof(int));
      MPI_Recv ( A, n, MPI_INT, my_rank - potencia(2, logaritmo2(my_rank)), A_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
      MPI_Recv ( B, n, MPI_INT, my_rank - potencia(2, logaritmo2(my_rank)), B_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

      for ( int i = logaritmo2(my_rank) + 1; i < logaritmo2(p); i++ ) {
        MPI_Send ( &n, 1, MPI_INT, my_rank + potencia(2 , i), N_TAG, MPI_COMM_WORLD );
        MPI_Send ( A, n, MPI_INT, my_rank + potencia(2 , i), A_TAG, MPI_COMM_WORLD );
        MPI_Send ( B, n, MPI_INT, my_rank + potencia(2 , i), B_TAG, MPI_COMM_WORLD );
      }
    }

    if(n == 0) break;

    localDotProduct = 0;
    lwr_lim = (my_rank)*(n/p);
    upr_lim = lwr_lim+(n/p);

    #pragma omp parallel for reduction(+:localDotProduct)
    for (i = lwr_lim; i < upr_lim; i++ ){
      localDotProduct += A[i]*B[i];
    }

    if ( logaritmoAux(my_rank) < logaritmo2(p)){
      dotProduct = 0;
      dotProduct += localDotProduct;

      for (i = logaritmo2(p) - 1; i >= logaritmoAux(my_rank); i--){
        MPI_Recv ( &rank_dot, 1, MPI_INT, my_rank + potencia(2, i), DOT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        dotProduct += rank_dot;
      }

      if(my_rank != ROOT_PROCESS){
        MPI_Send ( &dotProduct, 1, MPI_INT, my_rank - potencia(2,logaritmo2(my_rank)), DOT_TAG, MPI_COMM_WORLD );
      }
    }

    else{
      MPI_Send ( &localDotProduct, 1, MPI_INT, my_rank - potencia(2,logaritmo2(my_rank)), DOT_TAG, MPI_COMM_WORLD );
    }

    if ( my_rank == ROOT_PROCESS){
      printf("Dot product: %d\n", dotProduct);
    }
  }
  

  MPI_Finalize ();
  return 0;

}