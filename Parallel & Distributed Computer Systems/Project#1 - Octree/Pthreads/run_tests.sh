# A simple bash script that for running experiments
# Note: To run the script make sure it you have execution rights 
# (use: chmod u+x run_tests.sh to give execution rights) 
!/bin/bash

NAME=$(hostname)
DATE=$(date "+%Y-%m-%d-%H:%M:%S")
FILE_PREF=$NAME-$DATE-test-tree-pthreads

echo $NAME
echo $DATE

make clean; make
# run cube experiments
for N in 1048576 2097152 4194304 8388608 16777216 33554432; do \
    for thr in 2 4 8 16 32 64 128 256 512 1024 2048; do \
            echo cube N=$N && ./test_octree $N 1 128 3 18 $thr>> $FILE_PREF-cube.txt ; \
    done ; \
done ;
# run octant experiments
for N in 1048576 2097152 4194304 8388608 16777216 33554432; do \
    for thr in 2 4 8 16 32 64 128 256 512 1024 2048; do \
           echo Plummer N=$N && ./test_octree $N 1 128 10 18 $thr>> $FILE_PREF-plummer.txt ; \
    done ; \
done ;
