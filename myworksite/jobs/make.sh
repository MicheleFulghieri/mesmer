#!/bin/bash

#SBATCH --nodes=1 
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4         
#SBATCH --mem=60G                   
#SBATCH --time=05:00:00             
#SBATCH -J MakeMesmer
#SBATCH -p long                 
#SBATCH -o /data/mfulghieri/mesmer/outputs/compilation/make.out
#SBATCH -e /data/mfulghieri/mesmer/outputs/compilation/make.err


source $(conda info --base)/etc/profile.d/conda.sh
conda activate mesmer

module load gcc-11.3.0/ompi-4.1.4_nccl
module load cmake-3.22.1

cd /data/mfulghieri/mesmer/

# Compilation
make -j 30