#!/bin/sh

#PBS -q u-lecture
#PBS -N test
#PBS -l select=1:ncpus=15
#PBS -Wgroup_list=gt07
#PBS -l walltime=00:10:00
#PBS -e test2-15.err
#PBS -o test2-15.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export OMP_NUM_THREADS=15
export KMP_AFFINITY=granularity=fine,compact

numactl ./L3-rsol0 INPUT-15.DAT
