#!/bin/sh

#PBS -q u-lecture
#PBS -N test
#PBS -l select=1:ncpus=12
#PBS -Wgroup_list=gt07
#PBS -l walltime=00:10:00
#PBS -e test2-12.err
#PBS -o test2-12.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export OMP_NUM_THREADS=12
export KMP_AFFINITY=granularity=fine,compact

numactl ./L3-rsol0 INPUT-12.DAT
