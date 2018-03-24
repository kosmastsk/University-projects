function block = iBlockDCT(dctBlock)
% IBLOCKDCT is the inverse function from blockDCT.
% It receives an input the DCT coefficients of a block 8x8 and returns the
% block
% The transformation is applied in an block of size 8x8
% The procedure is described in 2-D.6.3.3 in MPEG-1 documentation (p.87)

block = idct2(dctBlock); 