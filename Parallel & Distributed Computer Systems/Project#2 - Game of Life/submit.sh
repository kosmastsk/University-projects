#PBS -N game-of-life
#PBS -q pdlab
#PBS -j oe
#PBS -l nodes=2:ppn=8

module load mpi/mpich3-x86_64

cd $PBS_O_WORKDIR

echo "==== Run starts now ======= `date` "

mpiexec -np 2 ./bin/game-of-life 80000 40000 0.5 0 3 &> $PBS_JOBNAME.log

echo "==== Run ends now ======= `date` "

