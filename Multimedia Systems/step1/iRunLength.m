function qBlock = iRunLength(runSymbols)
% IRUNLENGTH calculates the qBlock by a list of run symbols, that occured from the fzig-zag
% method and Run Length Encoding. qBlock's size is 8x8
% runSymbols is in the following format: (precedingZeros, quantSymbol)
% It this implementation we also include the DC coefficient, which is
% different to what the MPEG-1 does. So the first value will be: [0, DC_value]
% 2-D.6.3.4 (p.90) & 2-D.6.4.6 (p. 102) in MPEG-1 documentation

%% Inverse the Run Length Encoding
% Save the result in an array and later on transform it to a matrix
% qBlock's size is 8x8, since this is the the block we apply the DCT
% transform earlier. So the array we want to create is of size 8x8 = 64.

% Calculate the number of elements on the array, which is equal to the
% number of preceding zeros plus the number of quant symbols.
numElems = sum(runSymbols(:,1)) + size(runSymbols, 1);
% If that number is less or equal to 64, then we have 8x8 array, else is a
% Y channel, so 16x16
if numElems <= 64 
    qBlockSize = [8,8];
else
    qBlockSize = [16, 16];
end

irle = zeros(1, qBlockSize(1) * qBlockSize(2));
irle_index = 1;
R = size(runSymbols, 1);

for r = 1 : R
    for cz = 1 : runSymbols(r, 1)
        irle(irle_index) = 0;
        irle_index = irle_index + 1;
    end
    irle(irle_index) = runSymbols(r, 2);
    irle_index = irle_index + 1;
end


%% Undo the zig-zag scanning to create the qBlock

row = round(sqrt(length(irle)));

qBlock = zeros(row, row);
count=1;

for s = 1 : row
    if mod(s, 2) == 0
        for m = s : -1 : 1
            qBlock(s+1-m, m) = irle(count);
            count = count + 1;
        end
    else
        for m = 1 : s
            qBlock(s+1-m, m) = irle(count);
            count = count + 1;
        end
    end
end

if mod(row,2) == 0
    flip = 1;
else
    flip = 0;
end

for s = row+1 : 2*row-1
    if mod(flip, 2) == 0
        for m = row : -1 : s+1-row
            qBlock(s+1-m, m) = irle(count);
            count = count + 1;
        end
    else
        for m = row : -1 : s+1-row
            qBlock(m, s+1-m) = irle(count);
            count = count + 1;
        end
    end
    flip = flip + 1;
end

%% END
