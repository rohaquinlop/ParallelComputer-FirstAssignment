#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define ERROR_CODE    -1

#define ROOT_PROCESS  0

#define N_TAG         0
#define A_TAG         1
#define B_TAG         2
#define DOT_TAG       3

int main(int argc, char *argv[]){
  int* A;
  int* B;
  int i, n, my_rank, p, rank;
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
      //fflush ( stdout );
      scanf ("%d", &n );

      A = (int*) malloc(n*sizeof(int));
      B = (int*) malloc(n*sizeof(int));

      printf ("Enter the first vector values\n");
      //fflush ( stdout );
      for(i = 0; i < n; i++){
        scanf ("%d", &A[i] );
        //fflush ( stdout );
      }

      printf ("Enter the second vector values\n");
      //fflush ( stdout );
      for(i = 0; i < n; i++){
        scanf ("%d", &B[i] );
        //fflush ( stdout );
      }
      
      //Send information

      for(rank = 1; rank < p; rank++){
        MPI_Send ( &n, 1, MPI_INT, rank, N_TAG, MPI_COMM_WORLD );
        MPI_Send ( A, n, MPI_INT, rank, A_TAG, MPI_COMM_WORLD );
        MPI_Send ( B, n, MPI_INT, rank, B_TAG, MPI_COMM_WORLD );
      }
    }
    else{
      //Recieve information

      MPI_Recv ( &n, 1, MPI_INT, ROOT_PROCESS, N_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );

      A = (int*) malloc(n*sizeof(int));
      B = (int*) malloc(n*sizeof(int));

      MPI_Recv ( A, n, MPI_INT, ROOT_PROCESS, A_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
      MPI_Recv ( B, n, MPI_INT, ROOT_PROCESS, B_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }

    if(n == 0) break;

    localDotProduct = 0;
    lwr_lim = (my_rank)*(n/p);
    upr_lim = lwr_lim+(n/p);

    for (i = lwr_lim; i < upr_lim; i++ ){
      localDotProduct += A[i]*B[i];
    }

    if(my_rank == ROOT_PROCESS){
      dotProduct = 0;
      dotProduct += localDotProduct;

      for (rank = 1; rank < p; rank++ ){
        MPI_Recv ( &rank_dot, 1, MPI_INT, rank, DOT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        dotProduct += rank_dot;
      }

      printf("Dot product: %d\n", dotProduct);
    }
    else{
      MPI_Send ( &localDotProduct, 1, MPI_INT, ROOT_PROCESS, DOT_TAG, MPI_COMM_WORLD );
    }
  }
  

  MPI_Finalize ();
  return 0;

}