function [mBY, mBCr, mBCb] = iMotEstP(eMBY, eMBCr, eMBCb, mBIndex, mV, refFrameY, refFrameCr, refFrameCb)
% IMOTESTP is the inverse function of motEstP 
% as input it get the error macroblocks, the reference frame and the motion
% vector, and the output is the reconstructed macroblock

% disp('Hello from imotEstP');

%% Initialization

MBYSize = 16; % Macroblock size for Y frame
MBCSize = 8; % Macroblock size for chroma frames
[numberOfRows, numberOfCols] = size(refFrameY);

%% Since we have the MB index, we need to calculate where it is in the frame
% Calculate the indexing in rows and columns inside the frame of the macroblock

h = fix(mBIndex / (numberOfCols / 16)); % find the row
c = mod(mBIndex, numberOfCols / 16) ; % find the column

% Translate the row and column into actual pixel index values from the original frame

frameRow = h*MBYSize + 1 : h*MBYSize + MBYSize;
frameCol = c*MBYSize + 1 : c*MBYSize + MBYSize;

% Position of the chroma channels
frameRowChr = h*MBCSize + 1 : h*MBCSize + MBCSize;
frameColChr = c*MBCSize + 1 : c*MBCSize + MBCSize;

% Use the motion vector to move the macroblock to the direction needed on
% the reference frame
frameRow = frameRow + mV(2,1); % Move up or down, depending on the value ( + or -)
frameCol = frameCol + mV(1,1); % Move right or left, depending the value ( + or -)

frameRowChr = frameRowChr + mV(2,1); % Move up or down, depending on the value ( + or -)
frameColChr = frameColChr + mV(1,1); % Move right or left, depending the value ( + or -)

% Avoid the case there the index goes negative or more than the image limit
frameRow(frameRow < 1) = 1;
frameRow(frameRow > numberOfRows) = numberOfRows;

frameCol(frameCol < 1) = 1;
frameCol(frameCol > numberOfCols) = 1;

frameRowChr(frameRowChr < 1) = 1;
frameRowChr(frameRowChr > numberOfRows / 2) = numberOfRows / 2;

frameColChr(frameColChr < 1) = 1;
frameColChr(frameColChr > numberOfCols / 2) = numberOfCols / 2;

% We need to isolate the macroblock from the reference frame that will be
% used for reconstructing the initial macroblock
refMBY = refFrameY(frameRow, frameCol);
refMBCr = refFrameCr(frameRowChr, frameColChr);
refMBCb = refFrameCb(frameRowChr, frameColChr);

% Restore the initial macroblock using the following:
% Block = Error + refBlock
mBY = refMBY + eMBY;
mBCr = refMBCr + eMBCr;
mBCb = refMBCb + eMBCb;

% figure;
% imshow(mBY)
% title(['Macroblock ' num2str(mBIndex) '- Y channel after inverse motion estimation- coastguard001.tiff']);