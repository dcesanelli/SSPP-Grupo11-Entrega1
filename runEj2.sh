#!/bin/bash
#SBATCH -N 1
#SBATCH --exclusive
#SBATCH -o output.txt
#SBATCH -e errores.txt
./ejercicio2 $1 $2

