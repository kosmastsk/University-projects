function dctBlock = blockDCT(block)
% BLOCKDCT performs the DCT transformation.
% It receives as an input the input block and returns the DCT coefficients
% of the block 8x8
% The transformation is applied in an block of size 8x8
% The procedure is described in 2-D.6.3.3 in MPEG-1 documentation (p.87)

dctBlock = dct2(block); 