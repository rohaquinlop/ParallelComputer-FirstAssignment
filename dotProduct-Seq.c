#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]){
  int* A;
  int* B;
  int i, n;
  int res;

  for(;;){
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

    if ( n == 0 ) break;

    res = 0;
    for (i = 0; i < n; i++ ){
      res += A[i]*B[i];
    }

    printf("Dot product: %d\n", res);
  }

  return 0;
}