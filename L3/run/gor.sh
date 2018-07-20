#!/bin/sh

#PBS -q u-lecture
#PBS -N test
#PBS -l select=1:ncpus=16
#PBS -Wgroup_list=gt00
#PBS -l walltime=00:10:00
#PBS -e testx0.err
#PBS -o testx0.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export OMP_NUM_THREADS=16
export KMP_AFFINITY=granularity=fine,compact

numactl ./L3-rsol0
numactl ./L3-rsol0
numactl ./L3-rsol0
