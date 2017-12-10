function [a, b, k, n] = bisect_der(f, ak, bk, l, ab )
%BISECT_DER tries to find the minimum value of the function f.
%To use this function we must know that f has a derivative
%and by knowing the value of f'(xk), we can dicrease the initial interval
% into smaller intervals and finally get the minimun value x*.

df=diff(f,'x');
k=1;
n=log2((bk-ak)/l);
if ab==1
    figure
    plot(k,ak,'bo',k,bk,'ro');
    hold on;
end
while k~=sup(n)
    xk=(ak+bk)/2;
    value=subs(df,xk); %keep the value of f'(xk) - calculate it only once
    if (value==0)
        break;
    elseif (value>0)
        %ak remains the same
        bk=xk;
    else
        ak=xk;
        %bk remains the same
    end
    if ab==1
        plot(k,ak,'bo',k,bk,'ro');
        hold on;
    end
    k=k+1;
end
a=ak;
b=bk;
k=k-1;
n=sup(n);
if ab==1
    plot(k,ak,'bo',k,bk,'ro');
    hold on;
    grid on;
    xlabel('k');
    ylabel('[a,b]');
end

end

