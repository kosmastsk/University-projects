function dctBlock = dequantizeI(qBlock, qTable, qScale)
% DEQUANTIZEI is performing the inverse quantization of the I-frames.
% Procedure described at 2-D.6.3.4 in the MPEG-1 documentation (p. 89)
% qBlock contains the quantized coefficients of the DCT coefficients of the
% block. 
% dctBlock coontains the DCT coefficients
% qTable is the quantizer table  (2-D.6.3.4)
% qScale is the quantizer scale  (2-D.6.4.5)
% The result is a symbol and not a number.

% The DC coefficient is quantized differently and independently of the
% scale
% The DC coefficient is the top left pel
% The quantizer step size for the DC coefficients is fixed at 8

dctBlock = zeros(size(qBlock));

dctBlock(1,1) = (qBlock(1,1)) * 8;

for i = 1 : size(qBlock, 1)
    for j = 1 : size(qBlock, 2)
        if ( i == 1 && j == 1) 
            % This pel has already got a value - DC coeff
            continue;
        end
        dctBlock(i, j) = (qScale * qTable(i, j) * (qBlock(i, j))) / 8; % exactly the inverse procedure
    end
end