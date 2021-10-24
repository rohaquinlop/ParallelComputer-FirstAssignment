mpicc -Wall -fopenmp $1 -o ./Ejecutables/"${1/.c/.out}"
export OMP_NUM_THREADS=$2
mpiexec -use-hwthread-cpus -n $2 ./Ejecutables/"${1/.c/.out}" < ./Inputs/"${3}.txt"