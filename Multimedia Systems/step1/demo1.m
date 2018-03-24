%% Demo 1
% This script demostrates the functionality for the first part of the
% project - MPEG Library

%% Initialize the demo
clear all;
close all;
clc;

%% Get a list of all files in the folder with the desired file name pattern.

% Specify the folder where the files live.
% images available in the ../../coastguard-tiffs directory
myFolder = '../../coastguard-tiffs';
if ~isdir(myFolder)
  errorMessage = sprintf('Error: The following folder does not exist:\n%s', myFolder);
  uiwait(warndlg(errorMessage));
  return;
end

filePattern = fullfile(myFolder, '*.tiff');
images = dir(filePattern);

%% Preprocessing
% Read every image in the folder specified
for k = 1 : 1 % Change to 1 if we want to run for one image
    baseFileName = images(k).name;
    fullFileName = fullfile(myFolder, baseFileName);
    fprintf(1, 'Now reading %s\n', fullFileName);
    image = imread(fullFileName);
    figure;
    imshow(image);
    title('Original image');

%     Convert the image to YCrCb
    [frameY, frameCr, frameCb] = ccir2ycrcb(image);
   
%     Inverse function
    frameRGB = ycrcb2ccir(frameY, frameCr, frameCb);
      %The frame that occurs from frameRGB is exactly like the initial image
end

%% Motion Estimator for P frames
    
% Read the image in which to apply motion estimation "coastguard003.tiff"
baseFileName = images(4).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
image = imread(fullFileName);
figure;
imshow(image);
title('Image in which we apply motion estimation');
% Convert the image to YCrCb
[frameY, frameCr, frameCb] = ccir2ycrcb(image);

% Read the reference image "coastguard001.tiff"
baseFileName = images(2).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
refImage = imread(fullFileName);
figure;
imshow(image);
title('Reference image for motion estimation');
% Convert the image to YCrCb
[refFrameY, refFrameCr, refFrameCb] = ccir2ycrcb(refImage);

% frameY now is 360 pixels wide, but we need 352 to apply a 16x16
% macroblock size, so we need to delete 4 pixels in each side right and left. The same for the chroma
% frames, 2 pixels in each side

frameY = frameY(:, 5:356);

frameCr = frameCr(:, 3:178);

frameCb = frameCb(:, 3:178);
  
% The number of 16x16 macroblocks that can fit our image
max_mBIndex = floor(size(frameY, 1) / 16) * floor(size(frameY, 2) / 16);

for mBIndex = 0 : 1 % minus 1 since we start counting from zero
    
    % motion estimation P frames
    [eMBY, eMBCr, eMBCb, mV] = motEstP(frameY, frameCr, frameCb, mBIndex, refFrameY, refFrameCr, refFrameCb);   
    
    % Inverse motion estimation P frames
    [mBY, mBCr, mBCb] = iMotEstP(eMBY, eMBCr, eMBCb, mBIndex, mV, refFrameY, refFrameCr, refFrameCb);
end

%% Motion Estimator for B frames
    
% Read the image in which to apply motion estimation "coastguard003.tiff"
baseFileName = images(4).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
image = imread(fullFileName);
% Convert the image to YCrCb
[frameY, frameCr, frameCb] = ccir2ycrcb(image);

% Read the forward frame"coastguard001.tiff" % picture in the past
baseFileName = images(2).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
forwFrame = imread(fullFileName);
% Convert the image to YCrCb
[forwFrameY, forwFrameCr, forwFrameCb] = ccir2ycrcb(forwFrame);

% Read the backward frame "coastguard004.tiff" % picture in the future
baseFileName = images(5).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
backwFrame = imread(fullFileName);
% Convert the image to YCrCb
[backwFrameY, backwFrameCr, backwFrameCb] = ccir2ycrcb(backwFrame);

% frameY now is 360 pixels wide, but we need 352 to apply a 16x16
% macroblock size, so we need to delete 4 pixels in each side right and left. The same for the chroma
% frames, 2 pixels in each side

frameY = frameY(:, 5:356);

frameCr = frameCr(:, 3:178);

frameCb = frameCb(:, 3:178);
  
% The number of 16x16 macroblocks that can fit our image
max_mBIndex = floor(size(frameY, 1) / 16) * floor(size(frameY, 2) / 16);

for mBIndex = 1 : max_mBIndex - 1 % minus 1 since we start counting from zero
    % motion estimation B frames
    [eMBY, eMBCr, eMBCb, mV, MBY] = motEstB(frameY, frameCr, frameCb, mBIndex, backwFrameY, backwFrameCr, backwFrameCb, forwFrameY, forwFrameCr, forwFrameCb);

    % Inverse motion estimation B frames
    [mBY, mBCr, mBCb] = iMotEstB(eMBY, eMBCr, eMBCb, mBIndex, mV, backwFrameY, backwFrameCr, backwFrameCb, forwFrameY, forwFrameCr, forwFrameCb);
end

%% All the next function are applied to a specific macroblock of an image to prove their functionality
% We are using every function the one after the other, and in the end the
% exactly opposite procedure
      
% Read the image in which to apply motion estimation "coastguard003.tiff"
baseFileName = images(4).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
image = imread(fullFileName);
% Preprocessing
[frameY, frameCr, frameCb] = ccir2ycrcb(image);

% Read the reference image "coastguard001.tiff"
baseFileName = images(2).name;
fullFileName = fullfile(myFolder, baseFileName);
fprintf(1, 'Now reading %s\n', fullFileName);
refImage = imread(fullFileName);
% Preprocessing
[refFrameY, refFrameCr, refFrameCb] = ccir2ycrcb(refImage);

frameY = frameY(:, 5:356);

frameCr = frameCr(:, 3:178);

frameCb = frameCb(:, 3:178);
  
% The number of 16x16 macroblocks that can fit our image
max_mBIndex = floor(size(frameY, 1) / 16) * floor(size(frameY, 2) / 16);

% Apply it for every macroblock of the image
for mBIndex = 0 : 0 % max_mBIndex - 1 % minus 1 since we start counting from zero
    
    % motion estimation P frames
    [eMBY, eMBCr, eMBCb, mV] = motEstP(frameY, frameCr, frameCb, mBIndex, refFrameY, refFrameCr, refFrameCb);   
 
    % DCT transform
    % Apply the transformation in blocks of size 8x8
    for i = 1 : 8 : size(eMBY, 2)
        for j = 1 : 8 : size(eMBY, 1)
            eMBYDCT(j : j + 7, i : i + 7) = blockDCT(eMBY(j : j + 7, i : i + 7));
        end
    end
    
    % Cr and Cb are already in size 8x8 so we do not need any change
    eMBCrDCT = blockDCT(eMBCr);
    eMBCbDCT = blockDCT(eMBCb);
    
    qScale = 1/64; % This value may change if we want to
    % The following qTable comes from wikipedia, as a common quantization
    % matrix, just to test the functionality of our program
    qTable = [16 11 10 16 24 40 51 61; 
            12 12 14 19 26 58 60 55;
            14 13 16 24 40 57 69 56; 
            14 17 22 29 51 87 80 62;
            18 22 37 56 68 109 103 77;
            24 35 55 64 81 104 113 92;
            49 64 78 87 103 121 120 101;
            72 92 95 98 112 100 103 99];
    
    % Quantize - Apply either P/B quantize or I quantize, since here is
    % just for demostration of working
    for i = 1 : 8 : size(eMBY, 2)
        for j = 1 : 8 : size(eMBY, 1)
            quantBlockY(j : j + 7, i : i + 7) = quantizePB(eMBYDCT(j : j + 7, i : i + 7), qTable, qScale);
        end
    end
    
    quantBlockCb = quantizePB(eMBCbDCT, qTable, qScale);
    quantBlockCr = quantizePB(eMBCrDCT, qTable, qScale);

    % RLE
    runSymbolsY = runLength(quantBlockY);     
    runSymbolsCb = runLength(quantBlockCb);
    runSymbolsCr = runLength(quantBlockCr);
    
    % VLC
    vlcStreamY = vlc(runSymbolsY);
    vlcStreamCb = vlc(runSymbolsCb);
    vlcStreamCr = vlc(runSymbolsCr);
    
%% Inverse procedure
    
    % iVLC
    irunSymbolsY = ivlc(vlcStreamY);
    irunSymbolsCb = ivlc(vlcStreamCb);
    irunSymbolsCr = ivlc(vlcStreamCr);
    
    % iRunLength
    iquantBlockY = iRunLength(runSymbolsY);
    iquantBlockCb = iRunLength(runSymbolsCb);
    iquantBlockCr = iRunLength(runSymbolsCr);
    
%     ieMBYDCT = zeros(size(iquantBlockY))
    
    % Dequantize
    for i = 1 : 8 : size(eMBY, 2)
        for j = 1 : 8 : size(eMBY, 1)
            ieMBYDCT(j : j + 7, i : i + 7) = dequantizePB(iquantBlockY(j : j + 7, i : i + 7), qTable, qScale);
        end
    end
%     ieMBYDCT = dequantizePB(iquantBlockY, qTable, qScale);
    ieMBCbDCT = dequantizePB(iquantBlockCb, qTable, qScale);
    ieMBCrDCT = dequantizePB(iquantBlockCr, qTable, qScale);
    
    % iDCT
    % Apply the transformation in blocks of size 8x8
    for i = 1 : 8 : size(eMBY, 2)
        for j = 1 : 8 : size(eMBY, 1)
            ieMBY(j : j + 7, i : i + 7) = iBlockDCT(ieMBYDCT(j : j + 7, i : i + 7));
        end
    end
%     ieMBY = iBlockDCT(ieMBYDCT);
    % Cr and Cb are already in size 8x8 so we do not need any change
    ieMBCr = iBlockDCT(ieMBCrDCT);
    ieMBCb = iBlockDCT(ieMBCbDCT);
    
    % Inverse motion estimation P frames
    [mBY, mBCr, mBCb] = iMotEstP(ieMBY, ieMBCr, ieMBCb, mBIndex, mV, refFrameY, refFrameCr, refFrameCb);
    figure;
    imshow(mBY);
    title('Reconstructed macroblock 0 - Y channel - “coastguard003.tiff”');
end

%%  END 