function [h]=vlcTable(codeArray)
%generates the inverse vlc tree, stored in a table. It is used for
%decoding.
%codeArray: the input matrix, in the form of a cell array, with 
%each cell containing codewords represented in string format (array of chars) 




%Decoding logic (how to use this matrix in iVLC): If we are at row k and bit==0, go to row h(k,1) else if bit==1 go to row h(k,2).
%But if (h(k,1)==0 && h(k,2)==0) then a valid vlc word was found. The index of the found
%word is stored at h(k,3). Only leaf states (those corresponding to valid words have
%non-zero h(k,3);

h=zeros(1,3);
for codeIndex=1:length(codeArray)

    word=str2num([codeArray{codeIndex}]')';
    hIndex=1;

    hLength=size(h,1);
    for i=1:length(word)
        k=word(i)+1;
        if(~h(hIndex,k))
            h(hIndex,k)=hLength+1;
            hIndex=hLength+1;
            h(hIndex,1:2)=zeros(1,2);
            hLength=hLength+1;
        else
            hIndex=h(hIndex,k);
        end
    end
    h(hIndex,3)=codeIndex;
end