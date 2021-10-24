mpicc $1 -o ./Ejecutables/"${1/.c/.out}"
mpiexec -use-hwthread-cpus -n $2 ./Ejecutables/"${1/.c/.out}" < ./Inputs/"${3}.txt"