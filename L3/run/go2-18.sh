#!/bin/sh

#PBS -q u-lecture
#PBS -N test
#PBS -l select=1:ncpus=18
#PBS -Wgroup_list=gt07
#PBS -l walltime=00:10:00
#PBS -e test2-18.err
#PBS -o test2-18.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export OMP_NUM_THREADS=18
export KMP_AFFINITY=granularity=fine,compact

numactl ./L3-rsol0 INPUT-18.DAT
