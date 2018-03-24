function [eMBY, eMBCr, eMBCb, mV] = motEstP(frameY, frameCr, frameCb, mBIndex, refFrameY, refFrameCr, refFrameCb)
%MOTESTP estimates the move for P-frames
% 2-D.6.2 Motion Estimation and Compensation in the documentation (p.80)
% eMBY: 16x16 macroblock of estimation error -- luminance
% eMBCr, eMBCb: 8x8 macroblock of estimation error -- chroma
% mV: Motion vectors for the specific macroblocks. 2x2 array. For P-frames
% the 2nd column in NaN
% frameY, frameCr, frameCb: channels of the current P-frame
% mBIndex: index number of the macroblock
% refFrameY, refFrameCr, refFrameCb: channels for reference frames

% disp('Hello from motEstP');

%% WE ONLY CONSIDER THE LUMINANCE VALUES
%% Initialization

MBYSize = 16; % Macroblock size for Y channels
MBCSize = 8; % Macroblock size for chroma channels
w = 7; % Search parameter 
[numberOfRows, numberOfCols] = size(frameY);

% array to save the cost from each macroblock we check
% later on we will find the minimum of it
costs = ones(2*w + 1, 2*w +1) * 1;

% output arguments
mV = [NaN NaN; NaN NaN]; 

%% Since we have the MB index, we need to calculate where it is in the frame
% Calculate the indexing in rows and columns inside the frame of the macroblock

h = fix(mBIndex / (numberOfCols / 16)); % find the row
c = mod(mBIndex, numberOfCols / 16) ; % find the column

% Translate the row and column into actual pixel index values from the original frame
frameRow = (h*MBYSize + 1) : (h*MBYSize + MBYSize);
frameCol = (c*MBYSize + 1) : (c*MBYSize + MBYSize);

frameRowChr = (h*MBCSize + 1) : (h*MBCSize + MBCSize);
frameColChr = (c*MBCSize + 1) : (c*MBCSize + MBCSize);

MBY = frameY(frameRow , frameCol); % Extract only the macroblock we care for, from the original frame
MBCr = frameCr(frameRowChr, frameColChr); % Get the chroma pels to calculate the error later
MBCb = frameCb(frameRowChr, frameColChr);

figure;
imshow(MBY);
title(['Macroblock ' num2str(mBIndex) ' - Y channel - coastguard003.tiff']);

%% We are using exhaustive search. This means that we will calculate the error from 2w+1 blocks in each direction

% we will evaluate cost for  (2w + 1) blocks vertically and (2w + 1) blocks horizontaly
% m is row(vertical) index
% n is col(horizontal) index
% this means we are scanning in raster order

for m = -w : w
    for n = -w : w
        refBlkVer = frameRow(1) + m; % vertical co-ordinate for reference block
        refBlkHor = frameCol(1) + n; % Horizontal co-ordinate for reference block
        
        % skip the pels that are on the side and do not have the necessary amount of pels around to check
        if (refBlkVer < 1 || refBlkVer+MBYSize-1 > numberOfRows || refBlkHor < 1 || refBlkHor+MBYSize-1 > numberOfCols);
            continue;
        end
        % Calculate the cost using the Mean Absolute Difference (MAD)
        costs(m+w+1, n+w+1) = costFunction(MBY, refFrameY(refBlkVer:refBlkVer+MBYSize-1, refBlkHor:refBlkHor+MBYSize-1), MBYSize);
    end
end

% Find the place with the min cost
minCost = 99999;
for k = 1 : 2*w + 1
    for l = 1 : 2*w + 1
        if (costs(k, l) < minCost) % Attempted to access costs(1,38); index out of bounds because size(costs)=[41,37].
            minCost = costs(k, l);
            minX = l;
            minY = k;
        end
    end
end

% Set the motion Vector and the error that will be encoded and returned
mV(: , 1) = [minY - w - 1, minX - w - 1];

% E = MBY - macroblock withe min cost;
% If it tries to attempt a pel which is less than 1, it uses 1, so that it
% keeps in bounds

% Chroma indexes
chroma_row = frameRowChr + minY - fix(w/2) - 1;
chroma_col = frameColChr + minX - fix(w/2) - 1;

% keep them inside the picture size
chroma_row(chroma_row > numberOfRows / 2) = numberOfRows / 2;
chroma_row(chroma_row < 1) = 1;

chroma_col(chroma_col > numberOfCols / 2) = numberOfCols / 2;
chroma_col(chroma_col < 1) = 1;

figure;
imshow(refFrameY(max(1, frameRow + minY - w - 1), max(1, frameCol + minX - w - 1)));
title('the part of the image chosen using motEstP from coastguard001.tiff');

%% Calculate the error
eMBY = MBY - refFrameY(max(1, frameRow + minY - w - 1), max(1, frameCol + minX - w - 1));
eMBCr = MBCr - refFrameCr(chroma_row, chroma_col);
eMBCb = MBCb - refFrameCb(chroma_row, chroma_col);