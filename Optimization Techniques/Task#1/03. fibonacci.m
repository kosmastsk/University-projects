function [a, b, k, n] = fibonacci(f, ak, bk, l, ab)
%FIBONACCI tries to find the minimum x* of function f, where a<=x*<=b
%  using the fibonacci method, which dicreases the initial interval [a,b]
%  b multiplying with a different constant each time

if (l<=0)
    msg='Error occured! l must be greater than 0';
    error(msg)
end

if bk<=ak
    msg='Error occured! b must be greater than a';
    error(msg)
end

Fn=(bk-ak)/l;
fib=[];%save as many fibonacci numbers as needed 
%fib(0)=0 but MATLAB does not 
fib(1)=1;
fib(2)=1;
n=2;%n shows us how many fibonacci numbers will be needed
while fib(end)<Fn
    fib(end+1)=fib(end)+fib(end-1);
    n=n+1;
end
k=1;%counts the number of loops
if ab==1
    figure
    plot(k,ak,'ro',k,bk,'bo');
    hold on;
end
x1k=ak+(fib(n-k-1)/fib(n-k+1))*(bk-ak);
x2k=ak+(fib(n-k)/fib(n-k+1))*(bk-ak);
f1v=f(x1k);
f2v=f(x2k);
while k<n-1
    if f1v>f2v
        ak=x1k;
        %bk remains the same
        x1k=x2k;
        x2k=ak+(fib(n-k)/fib(n-k+1))*(bk-ak);
        f1v=f2v;
        f2v=f(x2k);
    else
        %ak remains the same
        bk=x2k;
        x2k=x1k;
        x1k=ak+(fib(n-k-1)/fib(n-k+1))*(bk-ak);
        f2v=f1v;
        f1v=f(x1k);
    end
    if ab==1
        plot(k,ak,'ro',k,bk,'bo');
        hold on;
    end
    k=k+1;
end
a=ak;
b=bk;
if ab==1
    plot(k,ak,'ro',k,bk,'bo');
    hold on;
    grid on;
    xlabel('k')
    ylabel('[a,b]')
end

end

