function line = FIRfilter(line, weights, div)
%FIRSUBSAMPLING This function applies an FIR filter, which is described by
%the weights 
% It applies to weights arrays with 7 values
% div is the value that the weights should be divided with

l = length(weights);

%get the index of the middle pel
middle = ceil(l/2);

for i = 1:length(line)

        % Check the cases where it tries to access pels out of the image
        % size and set these coefficients to zero == line(i)
        if ((i-1) < 1)
            a = line(i);
        else
            a = line(i-1);
        end
        
        if ((i-3) < 1)
            c = line(i);
        else
            c = line(i-3);
        end
        
        if ((i+1) > length(line))
            b = line(i);
        else
            b = line(i+1);
        end
        
        if ((i+3) > length(line))
            d = line(i);
        else
            d = line(i+3);
        end

        line(i) = (weights(middle)*line(i) + weights(middle-1) * (a + b) + weights(middle-3) * (c + d)); 
        line(i) = line(i) / div;
end

end