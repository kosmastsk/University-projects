%Bisection method
%define the functions
f1 = @(x) (x-2).^2+(x-5).^2;
f2 =@(x) (x-4).^2+cos(x);
f3=@(x) sqrt(x+1)+(x.^4-2).*log(x+1);

%1st question
%l=0.01 (constant) and we change e 
%get the plot between e and n 
l=0.01;
%f1(x)
figure;
for e=0.0005:0.00005:0.0045
    [a, b, k, n]=bisect(f1,0,6,l,e,0);
    plot(e, n, 'ok');
    hold on;
end
grid on;
xlabel('e');
ylabel('n');
title('f1(x), n as a function of e');

%f2(x)
figure;
for e=0.0005:0.00005:0.0045
    [a, b, k, n]=bisect(f2,0,6,l,e,0);
    plot(e, n, 'ok');
    hold on;
end
grid on;
xlabel('e');
ylabel('n');
title('f2(x), n as a function of e');

%f3(x)
figure;
for e=0.0005:0.00005:0.0045
    [a, b, k, n]=bisect(f3,0,6,l,e,0);
    plot(e, n, 'ok');
    hold on;
end
grid on;
xlabel('e');
ylabel('n');
title('f3(x), n as a function of e');

%2nd question
%e=0.001 (constant) and we change l
%get the plot between l and n 
e=0.001;
%f1(x)
figure;
for l=0.01:0.01:0.5
    [a, b, k, n]=bisect(f1,0,6,l,e,0);
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
    [a, b, k, n]=bisect(f2,0,6,l,e,0);
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
    [a, b, k, n]=bisect(f3,0,6,l,e,0);
    plot(l,n,'ok');
    hold on;
end
xlabel('l');
ylabel('n');
title('f3(x), n as a function of l');
grid on;

%3rd question
%l=0.01, 0.1, 0.2 and 0.4 while e=0.001 (constant)
%get a diagram of [a,b] and k 
%f1(x)
for l=[0.01 0.1 0.2 0.4]
    bisect(f1,0,6,l,e,1);
    hold on;
    str = sprintf('f1(x), l= %f, a and b as a function of l',l);
    title(str);
end

%f2(x)
for l=[0.01 0.1 0.2 0.4]
    bisect(f2,0,6,l,e,1);
    hold on;
    str = sprintf('f2(x), l= %f, a and b as a function of l',l);
    title(str);
end

%f3(x)
for l=[0.01 0.1 0.2 0.4]
    bisect(f3,0,6,l,e,1);
    hold on;
    str = sprintf('f3(x), l= %f, a and b as a function of l',l);
    title(str);
end

%end of 1st exercise