function [c, newpos] = readstream(x,len,pos);
%   x = content of a stream buffer. With integer values
%       in [0...255] corresponding to 8-bit words
%   len = the length (in bits) of the part of x to be read
%   pos = the absolute position of the first bit (in matlab 
%   notation, i.e., pos=1 for the first bit of x
%   c   = the extracted portion of the bitstream as a string of 1's and 0's
%   newpos = to be used as pos in subsequent readstream calls
%
%   see also: PUSHSTREAM

% Author: A. Delopoulos, 21.11.2010

WORDLENGTH = 8;
pastWholeWords = floor(pos/WORDLENGTH);
partWordBits = pos-pastWholeWords*WORDLENGTH-1;

cSpan = ceil(len/WORDLENGTH);
wordBuff = x(pastWholeWords+1:pastWholeWords+cSpan);
cBuff = dec2bin(double(wordBuff),WORDLENGTH);
cBuff = cBuff';
cBuff = cBuff(:);
c = cBuff(partWordBits+1:partWordBits+len)';
newpos = pos+len;
