function [] = all()

% execute all scripts in once

cd cilk
read_data('test-tree-cilk-cube.txt', 11, 6, 3);

cd
cd openMP
read_data('test-tree-openMP-cube.txt', 11, 6, 3);
cd..

cd Pthreads
read_data('test-tree-pthreads-cube.txt', 11, 6, 3);

end