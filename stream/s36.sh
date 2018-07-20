#!/bin/sh

#PBS -q u-lecture
#PBS -N stream
#PBS -l select=1:mpiprocs=36
#PBS -Wgroup_list=gt00
#PBS -l walltime=00:05:00
#PBS -e err
#PBS -o t36.lst

cd $PBS_O_WORKDIR
. /etc/profile.d/modules.sh

export I_MPI_PIN_PROCESSOR_LIST=0-35
mpirun ./impimap.sh ./stream
