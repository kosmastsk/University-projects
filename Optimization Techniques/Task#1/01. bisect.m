function [ak, bk, k, n] = bisect(f , a, b, l, e, ab)
%BISECT uses a constant e to break the initial interval [a,b]
%    into subintervals in order to find the minimum value x* of 
%    the function f, until b-a <= l, where l is the prescribed accuracy
%    and a<= x* <=b

k=1;
if e<=0
    msg='Error occured! e must be greater than 0';
    error(msg)
end 

if l<=0
    msg='Error occured! l must be greater than 0';
    error(msg)
end 

if b<=a
    msg='Error occured! b must be greater than a';
    error(msg)
end 

if ab==1 
    figure
end

while (b-a>=l)
    x1=(a+b)/2 - e;
    x2=(a+b)/2 + e;
    if (f(x1)<f(x2))
        %a remains the same
        b=x2;
    else
        a=x1;
        %b remains the same
    end
    if ab==1
        plot(k,a,'ro',k,b,'bo')
        hold on;
    end    
    k=k+1;
end
%the final interval is [ak,bk]
ak=a;
bk=b;
n=2*(k-1);
if ab==1
    plot(k,a,'ro',k,b,'bo');
    hold on;
    grid on;
    xlabel('k');
    ylabel('[a,b]');
end

end