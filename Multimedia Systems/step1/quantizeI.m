function qBlock = quantizeI(dctBlock, qTable, qScale)
% QUANTIZEI is performing the quantization of the I-frames.
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
qBlock = zeros(size(dctBlock));

qBlock(1,1) = (fix(dctBlock(1,1) / 8));

for i = 1 : size(dctBlock, 1)
    for j = 1 : size(dctBlock, 2)
        if ( i == 1 && j == 1) 
            % This pel has already got a value - DC coeff
            continue;
        end
        qBlock(i, j) = ( fix( ( 8 * dctBlock(i, j)) / (qScale * qTable(i, j)) ) );
    end
end
