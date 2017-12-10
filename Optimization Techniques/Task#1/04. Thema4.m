%Bisection with derivatives method
%define functions with a different way now, because we want the derivative
syms x;
f1=(x-2)^2+(x-5)^2;
f2=(x-4)^2+cos(x);
f3=sqrt(x+1)+(x^4-2)*log(x+1);

%1st question
%diagrams between l and n

%f1(x)
figure;
for l=0.01:0.01:0.5
    [a, b, k, n]=bisect_der(f1,0,6,l,0);
    plot(l,n,'ok');
    hold on;
end
xlabel('l');
ylabel('n');
title('f1(x), n as a function of l');
grid on;

%f2(x)
figure;
for l=0.01:0.01:0.5
    [a, b, k, n]=bisect_der(f2,0,6,l,0);
    plot(l,n,'ok');
    hold on;
end
xlabel('l');
ylabel('n');
title('f2(x), n as a function of l');
grid on;

%f3(x)
figure;
for l=0.01:0.01:0.5
    [a, b, k, n]=bisect_der(f3,0,6,l,0);
    plot(l,n,'ok');
    hold on;
end
xlabel('l');
ylabel('n');
title('f3(x), n as a function of l');
grid on;

%2nd question
% l=0.01, 0.1, 0.2 ??? 0.4 
%get a graph between [a,b] and k (k is the step counter) 

%f1(x)
for l=[0.01 0.1 0.2 0.4]
    bisect_der(f1,0,6,l,1);
    hold on;
    str = sprintf('f1(x), l= %f, a and b as a function of l',l);
    title(str);
end

%f2(x)
for l=[0.01 0.1 0.2 0.4]
    bisect_der(f2,0,6,l,1);
    hold on;
    str = sprintf('f2(x), l= %f, a and b as a function of l',l);
    title(str);
end

%f3(x)
for l=[0.01 0.1 0.2 0.4]
    bisect_der(f3,0,6,l,1);
    hold on;
    str = sprintf('f3(x), l= %f, a and b as a function of l',l);
    title(str);
end

%end of the 4th exercise