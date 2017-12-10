function [xmin, ymin, fmin, k] = SteepestDescent(e, xo, yo, ck)
%STEEPESTDESCENT is a method that tries to find the minimum value of function f(x,y)
% by taking steps proportional to the negative of the gradient of the function at the current point.
% e is the predemanded accuracy, xo and yo is the initial point and ck is
% the step value
if ck<0
    msg='Error occured! ck must be greater than 0';
    error(msg);
end
if e<0
    msg='Error occured! e must be greater than 0';
    error(msg);
end
k=1;
xk(k)=xo;
yk(k)=yo;
fk(k)=f(xk(k),yk(k));

while norm(gradf(xk(k),yk(k)))>e
    dk=-gradf(xk(k),yk(k));
    %used for the 2nd question-ck changes in order to minimize f(xk+ck*dk)
    h=@(ck) f(xk(k)+ck*dk(1),yk(k)+ck*dk(2));
    [ck1,ck2]=goldenSection(h,0,2,0.001);
    ck=(ck1+ck2)/2;
   
   %heuristic rule for changing ck
   
    xk(k+1)=xk(k)+ck*dk(1);
    yk(k+1)=yk(k)+ck*dk(2);
    fk(k+1)=f(xk(k+1),yk(k+1));
    k=k+1;
end
figure;
hold on;
grid on;
hold on;
xlabel('x');
hold on;
ylabel('y');
hold on;
zlabel('f(x,y)');
hold on;
plot3(xk,yk,fk, 'bo');
figure;
hold on;
for i=1:k
    plot(i,fk(i),'ro');
    hold on;
    grid on;
    hold on;
    xlabel('k');
    hold on;
    ylabel('fmin');
    hold on;
end

xmin=xk(k)
ymin=yk(k)
fmin=fk(k)

end
    
