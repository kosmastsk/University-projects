%Exercise 3 

%                               %
%get initial point (xo,yo)=(-0.5, -0.5);
[xmin, ymin, fmin, k]=CG(0.00001, -0.5, -0.5, 0.5);

%get initial point (xo,yo)=(0.5,0.5)
[xmin, ymin, fmin, k]=CG(0.00001, 0.5, 0.5, 0.5);

%get initial point (xo,yo)=(-2,1)
[xmin, ymin, fmin, k]=CG(0.00001, -2, 1, 0.5);

%get initial point (xo,yo)=(-2,1)
[xmin, ymin, fmin, k]=CG(0.00001, -3, 3, 0.5);