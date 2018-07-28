#!/bin/sh

#PBS -q u-lecture
#PBS -N test
#PBS -l select=1:ncpus=7
#PBS -Wgroup_list=gt07
#PBS -l walltime=00:10:00
#PBS -e test2-7.err
#PBS -o test2-7.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export OMP_NUM_THREADS=7
export KMP_AFFINITY=granularity=fine,compact

numactl ./L3-rsol0 INPUT-7.DAT
