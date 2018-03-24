function [mBY, mBCr, mBCb] = iMotEstB(eMBY, eMBCr, eMBCb, mBIndex, mV, backwFrameY, backwFrameCr, backwFrameCb, forwFrameY, forwFrameCr, forwFrameCb)
% IMOTESTB is the inverse function of motEstB
% as input it get the error macroblocks, the reference frames and the motion
% vector, and the output is the reconstructed macroblock

% disp('Hello from imotEstB');

%% Initialization

MBYSize = 16; % Macroblock size for Y frame
MBCSize = 8; % Macroblock size for chroma frames
[numberOfRows, numberOfCols] = size(backwFrameY);

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

%% For the forward prediction

% Use the motion vector to move the macroblock to the direction needed on
% the past frame
forwFrameRow = frameRow + mV(2,1); % Move up or down, depending on the value ( + or -)
forwFrameCol = frameCol + mV(1,1); % Move right or left, depending the value ( + or -)

forwFrameRowChr = frameRowChr + mV(2,1); % Move up or down, depending on the value ( + or -)
forwFrameColChr = frameColChr + mV(1,1); % Move right or left, depending the value ( + or -)

% Avoid the case there the index goes negative or more than the image limit
forwFrameRow(forwFrameRow < 1) = 1;
forwFrameRow(forwFrameRow > numberOfRows) = numberOfRows;

forwFrameCol(forwFrameCol < 1) = 1;
forwFrameCol(forwFrameCol > numberOfCols) = 1;

forwFrameRowChr(forwFrameRowChr < 1) = 1;
forwFrameRowChr(forwFrameRowChr > numberOfRows / 2) = numberOfRows / 2;

forwFrameColChr(forwFrameColChr < 1) = 1;
forwFrameColChr(forwFrameColChr > numberOfCols / 2) = numberOfCols / 2;

% We need to isolate the macroblock from the reference frame that will be
% used for reconstructing the initial macroblock
forwMBY = forwFrameY(forwFrameRow, forwFrameCol);
forwMBCr = forwFrameCr(forwFrameRowChr, forwFrameColChr);
forwMBCb = forwFrameCb(forwFrameRowChr, forwFrameColChr);

%% For the backward prediction

% Use the motion vector to move the macroblock to the direction needed on
% the future frame
backwFrameRow = frameRow + mV(2,2); % Move up or down, depending on the value ( + or -)
backwFrameCol = frameCol + mV(1,2); % Move right or left, depending the value ( + or -)

backwFrameRowChr = frameRowChr + mV(2,2); % Move up or down, depending on the value ( + or -)
backwFrameColChr = frameColChr + mV(1,2); % Move right or left, depending the value ( + or -)

% Avoid the case there the index goes negative or more than the image limit
backwFrameRow(backwFrameRow < 1) = 1;
backwFrameRow(backwFrameRow > numberOfRows) = numberOfRows;

backwFrameCol(backwFrameCol < 1) = 1;
backwFrameCol(backwFrameCol > numberOfCols) = 1;

backwFrameRowChr(backwFrameRowChr < 1) = 1;
backwFrameRowChr(backwFrameRowChr > numberOfRows / 2) = numberOfRows / 2;

backwFrameColChr(backwFrameColChr < 1) = 1;
backwFrameColChr(backwFrameColChr > numberOfCols / 2) = numberOfCols / 2;

% We need to isolate the macroblock from the reference frame that will be
% used for reconstructing the initial macroblock
backwMBY = backwFrameY(backwFrameRow, backwFrameCol);
backwMBCr = backwFrameCr(backwFrameRowChr, backwFrameColChr);
backwMBCb = backwFrameCb(backwFrameRowChr, backwFrameColChr);


%% Restore the initial macroblock using the following:
% Block = Error + 0.5 * (fordwBlock+backwBlock)
mBY = eMBY + 0.5*(backwMBY + forwMBY);
mBCr = eMBCr + 0.5*(backwMBCr + forwMBCr);
mBCb = eMBCb + 0.5*(backwMBCb + forwMBCb);

% figure;
% imshow(mBY)
