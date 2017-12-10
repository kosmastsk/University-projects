function [a, b] = goldenSection(f, a, b, l)
%GOLDENSECTION  tries to find the minimum value of the mathematical 
%  function f by multiplying the limits of the initial interval a or b 
%  with 0.618, until b-a =<l, where l is the prescribed acucuracy
if (l<=0)
    msg='Error occured! l must be greater than 0';
    error(msg)
end

if b<=a
    msg='Error occured! b must be greater than a';
    error(msg)
end

k=1;
ak=a;
bk=b;

c=0.618;
x1k=ak+(1-c)*(bk-ak);
x2k=ak+c*(bk-ak);
f1v=f(x1k);
f2v=f(x2k);
n=2;
while (bk-ak>=l)
    if (f1v>f2v)
        ak=x1k;
        %bk remains the same
        x2k=ak+c*(bk-ak);
        x1k=x2k;
        f1v=f2v;
        f2v=f(x2k);
		n=n+1;
    else
        %ak remains the same
        bk=x2k;
        x2k=x1k;
        x1k=ak+(1-c)*(bk-ak);
        f2v=f1v;
        f1v=f(x1k);
		n=n+1;
    end
    k=k+1;
end

a=ak;%return a
b=bk;%return b

end