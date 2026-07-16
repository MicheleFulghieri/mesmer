#!/bin/bash
#SBATCH --job-name=analysis
#SBATCH --cpus-per-task=48
#SBATCH --time=02:30:00
#SBATCH -o /data/mfulghieri/mesmer/myworksite/analysis/mue_mueg/outputs/log.out
#SBATCH -e /data/mfulghieri/mesmer/myworksite/analysis/mue_mueg/outputs/err.err


DIR="/data/mfulghieri/mesmer/myworksite/analysis/mue_mueg"
EXE="/data/mfulghieri/mesmer/myworksite/analysis/mue_mueg/analyze_mue_mueg.py"


cd "${DIR}"
python3 "${EXE}"

