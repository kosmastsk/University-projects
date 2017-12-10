function [xmin, ymin, fmin, k] = LMA(e, x0, y0, ck)
%LMA is the Llevenberg - Marquardt method that tries to minimize a function
%f(x,y) . It work like the Newton Method with the only difference that the
%hessian matrix of f is always positive definite

if e<0
    msg='Error occured! e must be greater than zero!';
    error(msg);
end
if ck<0
    msg='Error occured! ck must be greater than zero!';
    error(msg);
end

k=1;
xk(k)=x0;
yk(k)=y0;
fk(k)=f(xk(k),yk(k));
while norm(gradf(xk(k),yk(k)))>=e
    mk=abs(max(eig(hessianf(xk(k),yk(k))))) + 0.1;
    A=hessianf(xk(k),yk(k))+mk*eye(2);
    Z=(A+A')/2;
    while (eig(Z)<=0)
        mk=mk+0.1
        A=hessianf(xk(k),yk(k))+mk*eye(2)
    end
    dk=A\(-gradf(xk(k),yk(k)));
    
  %  used for the 2nd question-ck changes in order to minimize f(xk+ck*dk)
    h=@(ck) f(xk(k)+ck*(dk(1)/abs(dk(1))),yk(k)+ck*(dk(2)/abs(dk(2))));
   [ck1,ck2]=goldenSection(h,0,2,0.00001);
    ck=(ck1+ck2)/2;

   xk(k+1)=xk(k)+ck*(dk(1));
   yk(k+1)=yk(k)+ck*(dk(2));
   k=k+1;
   fk(k)=f(xk(k),yk(k));
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

