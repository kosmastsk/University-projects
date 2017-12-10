function y = sup(x)
%SUP calculates the supremum of the number x

if (double(int64(x))-x)<0
    y=int64(x)+1;
else 
    y=int64(x);
end

end

