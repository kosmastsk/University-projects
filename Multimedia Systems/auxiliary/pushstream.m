function [y,newoffset]=pushstream(c,x,offset);
%   c = binary row (e.g., c=[1 1 1 0 0 1] or c='111001' )
%   x = current content of the buffer. With integer values
%       in [0...255] corresponding to 8-bit words
%   offset = current offset, i.e., the position within the last 
%   word that should be written at the next "push".
%   offset is in the range [0...7] where 0 indicates the leftmost
%   i.e., msb of the last word. 
%   y, newoffset = new content of the buffer and new offset
%
%   example:
%   [x, n] = pushstream([0 1 1],[],0) %start with an empty stream
%   [y, m] = pushstream('1111001',x,n) %append 7 bits to stream x
%
%   The above calls generate in two steps the word stream y=[126  64]
%   corresponding to the binary sequence 011,11110 01,00000
%   with offset m=2 (i.e., third bit from the left). 
%   Commas are used just for visualization of 
%   original sequences limits, while the space is used for visualizing 
%   word (byte) limits. The 5 zeros after the second comma are stuffing
%   bits that will be replaced by some next call of pushstream().
%   Use dec2bin(double(y),8) to cross check.
%
%   see also: READSTREAM

% Author: A. Delopoulos, 21.11.2010

WORDLENGTH = 8;
if ischar(c)
    c = str2num(c(:))';
end

if isempty(c)
    y = x;
    newoffset = offset;
    return
end

if isempty(x)
    remainingPositions=WORDLENGTH;
    x = uint8(0);
else
remainingPositions = WORDLENGTH-offset;
end

if length(c)<=remainingPositions
    %first bit goes to msb
    cr = c(end:-1:1);
    
    tmp = 2^(remainingPositions-length(cr))*sum(bitset(0,find(cr)));
    tmp = tmp+double(x(end)); tmp = uint8(tmp);
    x(end) = tmp;
    y = x;
    newoffset = mod(offset+length(cr),WORDLENGTH);
    if (newoffset==0)
        y(length(y)+1) = 0;
    end
else
    % fill the remaining positions of the current word
    c1 = c(1:remainingPositions);
    %first bit goes to msb
    cr1 = c1(end:-1:1);
    tmp = 2^(remainingPositions-length(cr1))*sum(bitset(0,find(cr1)));
    tmp = tmp+double(x(end)); tmp=uint8(tmp);
    x(end)=tmp;
    % keep next bits for additional words
    c2 = c(remainingPositions+1:end);
    y = x;
    wholeWords = floor(length(c2)/WORDLENGTH);
    z = zeros(WORDLENGTH,wholeWords);
    z(:) = c2(1:WORDLENGTH*wholeWords);
    
    z(WORDLENGTH:-1:1,:) = z;
    
    n = 0:WORDLENGTH-1;   
    z = 2.^n*z;
    z = uint8(z);
    y = [y(:);z(:)];
    y(length(y)+1) = 0;
    [y,newoffset] = pushstream(c2(wholeWords*WORDLENGTH+1:length(c2)),y,0);
end

