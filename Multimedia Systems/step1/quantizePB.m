function qBlock = quantizePB(dctBlock, qTable, qScale)
% QUANTIZEPB is performing the quantization of the P and B frames.
% Procedure described at 2-D.6.3.4 in the MPEG-1 documentation (p. 89)
% qBlock contains the quantized coefficients of the DCT coefficients of the
% block. 
% dctBlock coontains the DCT coefficients
% qTable is the quantizer table  (2-D.6.3.4)
% qScale is the quantizer scale  (2-D.6.4.5)
% The result is a symbol and not a number.

% Non-intra macroblocks in P and B pictures are quantized using the quantizer scale and the non-intra quantization matrix.
% Both DC and the AC coefficients are quantized the same way.

qBlock = zeros(size(dctBlock));

for i = 1 : size(dctBlock, 1)
    for j = 1 : size(dctBlock, 2)
        qBlock(i, j) = fix( ( 8 * dctBlock(i, j)) / (qScale * qTable(i, j)) );
    end
end