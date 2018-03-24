function [eMBY, eMBCr, eMBCb, mV, MBY] = motEstB(frameY, frameCr, frameCb, mBIndex, backwFrameY, backwFrameCr, backwFrameCb, forwFrameY, forwFrameCr, forwFrameCb)
%MOTESTB estimates the move for B-frames
% 2-D.6.2 Motion Estimation and Compensation in the documentation (p.80)
% eMBY: 16x16 macroblock of estimation error -- luminance
% eMBCr, eMBCb: 8x8 macroblock of estimation error -- chroma
% mV: Motion vectors for the specific macroblocks. 2x2 array
% frameY, frameCr, frameCb: channels of the current P-frame
% mBIndex: index number of the macroblock
% backwFrameY, backwFrameCr, backwFrameCb: backward reference frames -
% future picture
% forwFrameY, forwFrameCr, forwFrameCb: forward reference frames - past
% picture

% disp('Hello from motEstB');

%% WE ONLY CONSIDER THE LUMINANCE VALUES
%% Initialization

MBYSize = 16; % Macroblock size for Y channels
MBCSize = 8; % Macroblock size for chroma channels
w = 7; % Search parameter 
[numberOfRows, numberOfCols] = size(frameY);

% array to save the cost from each macroblock we check
% later on we will find the minimum of it
backwCosts = ones(2*w + 1, 2*w +1);
forwCosts = ones(2*w + 1, 2*w +1);

% output arguments
mV = [NaN NaN; NaN NaN];

%% Since we have the MB index, we need to calculate where it is in the frame
% Calculate the indexing in rows and columns inside the frame of the macroblock

h = fix(mBIndex / (numberOfCols / 16)); % find the row
c = mod(mBIndex, numberOfCols / 16) ; % find the column

% Translate the row and column into actual pixel index values from the original frame
frameRow = h*MBYSize + 1 : h*MBYSize + MBYSize;
frameCol = c*MBYSize + 1 : c*MBYSize + MBYSize;

frameRowChr = h*MBCSize + 1 : h*MBCSize + MBCSize;
frameColChr = c*MBCSize + 1 : c*MBCSize + MBCSize;

MBY = frameY(frameRow , frameCol); % Extract only the macroblock we care for, from the original frame
MBCr = frameCr(frameRowChr, frameColChr); % Get the chroma pels to calculate the error later
MBCb = frameCb(frameRowChr, frameColChr);

% figure;
% imshow(MBY);
% title('Macroblock 0 - Y channel - coastguard003.tiff');

%% For the forward prediction

% We are using exhaustive search. This means that we will calculate the error from 2w+1 blocks in each direction

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
        forwCosts(m+w+1, n+w+1) = costFunction(MBY, forwFrameY(refBlkVer:refBlkVer+MBYSize-1, refBlkHor:refBlkHor+MBYSize-1), MBYSize);
    end
end

% Find the place with the min cost
forwMinCost = 1;
for k = 1 : 2*w + 1
    for l = 1 : 2*w + 1
        if (forwCosts(k, l) < forwMinCost) % Attempted to access costs(1,38); index out of bounds because size(costs)=[41,37].
            forwMinCost = forwCosts(k, l);
            forwMinX = l;
            forwMinY = k;
        end
    end
end

% Set the motion vector. First column for forward prediction
mV(: , 1) = [forwMinY - w - 1, forwMinX - w - 1];

% If it tries to attempt a pel which is less than 1, it uses 1, so that it
% keeps in bounds

% Chroma indexes
forw_chroma_row = frameRowChr + forwMinY - fix(w/2) - 1;
forw_chroma_col = frameColChr + forwMinX - fix(w/2) - 1;

% keep them inside the picture size
forw_chroma_row(forw_chroma_row > numberOfRows / 2) = numberOfRows / 2;
forw_chroma_row(forw_chroma_row < 1) = 1;

forw_chroma_col(forw_chroma_col > numberOfCols / 2) = numberOfCols / 2;
forw_chroma_col(forw_chroma_col < 1) = 1;

%% For the backward prediction

% We are using exhaustive search. This means that we will calculate the error from 2w+1 blocks in each direction

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
        backwCosts(m+w+1, n+w+1) = costFunction(MBY, backwFrameY(refBlkVer:refBlkVer+MBYSize-1, refBlkHor:refBlkHor+MBYSize-1), MBYSize);
    end
end

% Find the place with the min cost
backwMinCost = 1;
for k = 1 : 2*w + 1
    for l = 1 : 2*w + 1
        if (backwCosts(k, l) < backwMinCost)
            backwMinCost = backwCosts(k, l);
            backwMinX = l;
            backwMinY = k;
        end
    end
end

% Set the motion vector. Second column for backward prediction 
mV(: , 2) = [backwMinY - w - 1, backwMinX - w - 1];

% If it tries to attempt a pel which is less than 1, it uses 1, so that it
% keeps in bounds

% Chroma indexes
backw_chroma_row = frameRowChr + backwMinY - fix(w/2) - 1;
backw_chroma_col = frameColChr + backwMinX - fix(w/2) - 1;

% keep them inside the picture size
backw_chroma_row(backw_chroma_row > numberOfRows / 2) = numberOfRows / 2;
backw_chroma_row(backw_chroma_row < 1) = 1;

backw_chroma_col(backw_chroma_col > numberOfCols / 2) = numberOfCols / 2;
backw_chroma_col(backw_chroma_col < 1) = 1;

%% Calculate the error
% figure;
% imshow(backwFrameY(frameRow + backwMinY - w - 1, frameCol + backwMinX - w - 1));

eMBY = MBY - 0.5 * (backwFrameY(frameRow + backwMinY - w - 1, frameCol + backwMinX - w - 1) + forwFrameY(frameRow + forwMinY - w - 1, frameCol + forwMinX - w - 1));
eMBCr = MBCr - 0.5 * (backwFrameCr(backw_chroma_row, backw_chroma_col) + forwFrameCr(forw_chroma_row, forw_chroma_col));
eMBCb = MBCb - 0.5 * (backwFrameCb(backw_chroma_row, backw_chroma_col) + forwFrameCb(forw_chroma_row, forw_chroma_col));