function dctBlock = dequantizePB(qBlock, qTable, qScale)
% QUANTIZEPB is performing the inverse quantization of the P and B frames.
% Procedure described at 2-D.6.3.4 in the MPEG-1 documentation (p. 89)
% qBlock contains the quantized coefficients of the DCT coefficients of the
% block. 
% dctBlock coontains the DCT coefficients
% qTable is the quantizer table  (2-D.6.3.4)
% qScale is the quantizer scale  (2-D.6.4.5)
% The result is a symbol and not a number.

dctBlock = zeros(size(qBlock));

for i = 1 : size(qBlock, 1)
    for j = 1 : size(qBlock, 2)
        dctBlock(i, j) = (qScale * qTable(i, j) * (qBlock(i, j))) / 8; % exactly the inverse procedure
    end
end