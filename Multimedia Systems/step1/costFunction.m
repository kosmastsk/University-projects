function cost = costFunction(Block, refBlock, n)
% COSTFUNCTION calculates the cost of a macroblock for motion estimation
% Using the Mean Absolute Difference (MAD) for the given two blocks
% Input
% currentBlk : The block for which we are finding the MAD
% refBlk : the block w.r.t. which the MAD is being computed
% n : the side of the two square blocks

% disp('Hello from costFunction');
err = 0;
for i = 1:n
    for j = 1:n
        err = err + abs((Block(i,j) - refBlock(i,j)));
    end
end
cost = err / (n*n);