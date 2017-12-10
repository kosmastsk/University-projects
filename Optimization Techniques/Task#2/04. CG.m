function [xmin, ymin, fmin, k] = CG(e, x0, y0, ck)
%CG (Conjugate Gradient) tries to find the minimum of the function f using
%the statement: x(k+1)=x(k)+ck*dk and dk is equal to -gradf(xk)+bk*d(k-1)

k=1;
xk(k)=x0;
yk(k)=y0;
fmin=f(xk(k),yk(k));
d(k)=-gradf(xk(k),yk(k));
while gradf(xk(k),yk(k))>=e
    bk=((gradf(xk(k),yk(k))')*(gradf(xk(k),yk(k))-gradf(xk(k-1),yk(k-1)))/((gradf(xk(k-1),yk(k-1))')*gradf(xk(k-1),yk(k-1))));
    d(k+1)=-gradf(xk(k),yk(k))+bk*d(k);
    xk(k+1)=xk(k)+ck*d(k);
    yk(k+1)=yk(k)+ck*dk(k);
    fmin(k+1)=f(xk(k),yk(k));
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

