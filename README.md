# Parallel Computer

## First Assignment

### Grupo 2

- Londoño Cárdenas Jhon Sebastián
- Quintero Lopez Robin Hafid

### Correr los programas con las pruebas

```console
./compileMPI.sh $1 $2 $3
```

El primer parametro es el nombre del archivo c a compilar, el segundo parametro es la cantidad de hilos y el tercero es el nombre del archivo sin la extension .txt con la que se van a correr las pruebas.

```console
./compileMPI.sh dotProduct-MPI.c 2 512
```

```console
./compileOpenMP.sh dotProduct-OMP.c 2 512
```

```console
./compileMPI-OpenMP.sh dotProduct-BinomialTreeOMP.c 2 512
```

### Generar input

```console
./generateRandom.sh $1
```

El script recibe como parametro la cantidad de numeros aleatorios que se desean generar y guardar en la carpeta Inputs, el archivo generado queda con nombre *cantidad_generada*.txt

```console
./generateRandom.sh 1024
```
