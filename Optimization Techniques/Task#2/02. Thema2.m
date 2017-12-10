%Exercise 2 

%                                           %
%get initial point (xo,yo)=(-0.5, -0.5);
[xmin, ymin, fmin, k]=Newton(0.00001, -0.5, -0.5, 0.5);

%get initial point (xo,yo)=(0.5,0.5)
[xmin, ymin, fmin, k]=Newton(0.00001, 0.5, 0.5, 0.5);%z

%get initial point (xo,yo)=(-2,1)
[xmin, ymin, fmin, k]=Newton(0.0001, -2, 1, 0.5);

%get initial point (xo,yo)=(-2,1)
[xmin, ymin, fmin, k]=Newton(0.00001, -3, 3, 0.5);



