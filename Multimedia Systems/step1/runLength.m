function runSymbols = runLength(qBlock)
% RUNLENGTH calculates the list of run lengths by using the fzig-zag
% method to scan the qBlock. Its size is Rx2, where R is the number of run
% lengths
% Its result is in the following format: (precedingZeros, quantSymbol)
% It this implementation we also include the DC coefficient, which is
% different to what the MPEG-1 does. So the first value will be: [0, DC_value]
% 2-D.6.3.4 (p.90) & 2-D.6.4.6 (p. 102) in MPEG-1 documentation

%% Use zig-zag scanning to convert the matrix to an array

[row, col]=size(qBlock);

% zz will store the zig-zag format of the matrix
zz = zeros(row*col,1);

count=1;
for s=1:col
    if mod(s,2)==0
        for m=s:-1:1
            zz(count)=qBlock(s+1-m, m);
            count=count+1;
        end;
    else
        for m=1:s
            zz(count)=qBlock(s+1-m, m);
            count=count+1;
        end;
    end;
end;

if mod(row,2)==0
    flip=1;
else
    flip=0;
end;

for s=col+1:2*col-1
    if mod(flip,2)==0
        for m=row:-1:s+1-row
            zz(count)=qBlock(s+1-m, m);
            count=count+1;
        end;
    else
        for m=row:-1:s+1-row
            zz(count)=qBlock(m, s+1-m);
        count=count+1;
        end;    
    end;
    flip=flip+1;
end;

%% Run Length Encoding

cz = 0; % Consecutive zeros

index = 1; % index that will place the values in the runSymbols array

L = length(zz);

% The first column will be the run length
% Second column will be the level

% Firstly set the DC coefficient which is always the same
runSymbols(index, 1) = 0;
runSymbols(index, 2) = qBlock(1, 1);
index = index + 1;

for i = 2 : L % start from 2, since we already checked the first value
    if zz(i) == 0
        cz = cz + 1;
    else
        runSymbols(index, 1) = cz;
        runSymbols(index, 2) = zz(i);
        index = index + 1;
        cz = 0;
    end
end

%% END