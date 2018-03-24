function [frameY, frameCr, frameCb] = ccir2ycrcb(frameRGB)
%CCIR2YCRCB Create a sequence of images 352x288 in YCrCb 4:2:0
%   2-D.3.1 in MPEG video documentation (p. 57)

% disp('Hello from ccir2ycrrb function\n');
%% Initialize
% frameRGB is 576x720x3 struct

frameRGB = im2double(frameRGB); % Convert the image to double precision

R = frameRGB(:,:,1) ; % Red channel
G = frameRGB(:,:,2) ; % Green channel
B = frameRGB(:,:,3) ; % Blue channel

%% Convert the values from RGB to YCrCb pixels

frameY = uint8(floor(77*R + 150*G + 29*B));
frameCb = uint8(floor(((-44*R(:, 1:2:end) - 87*G(:, 1:2:end) + 131*B(:, 1:2:end))/256 + 0.5)*256));
frameCr = uint8(floor(((131*R(:, 1:2:end) - 110*G(:, 1:2:end) - 21*B(:, 1:2:end))/256 + 0.5)*256));

frameY = im2double(frameY);
frameCr = im2double(frameCr);
frameCb = im2double(frameCb);

% See the result of the initial convertion
% figure;
% imshow(frameY);
% title('frame Y');
% 
% figure;
% imshow(frameCr);
% title('frame Cr');
% 
% figure;
% imshow(frameCb);
% title('frame Cb');

%% Odd field only

frameY = downsample(frameY, 2);
frameCr = downsample(frameCr, 2);
frameCb = downsample(frameCb, 2);

%% Luminance Subsampling Filter Tap Weights

lum_tap_weights = [-29, 0, 88, 138, 88, 0, -29]; % //256

%% Chrominance Subsampling Filter Tap Weights

chr_tap_weights = [1, 0, 3, 0, 3, 0, 1]; % //8

%% Horizontal Filter and subsample

for w = 1 : size(frameY, 2)
    frameY(:, w) = FIRfilter(frameY(:, w), lum_tap_weights, 256);
end

for w = 1 : size(frameCr, 2)
    frameCr(:, w) = FIRfilter(frameCr(:, w), chr_tap_weights, 8);
    frameCb(:, w) = FIRfilter(frameCb(:, w), chr_tap_weights, 8);
end

frameY = frameY(: , 1:2:end);
frameCr = frameCr(: , 1:2:end);
frameCb = frameCb(: , 1:2:end);

%% Vertical Filter and subsample for frames Cr and Cb

for h = 1 : size(frameCr, 1)
    frameCr(h, :) = FIRfilter(frameCr(h, :), chr_tap_weights, 8);
    frameCb(h, :) = FIRfilter(frameCb(h, :), chr_tap_weights, 8);
end

frameCr = downsample(frameCr, 2); 
frameCb = downsample(frameCb, 2);

%% Hopefully SIF resolution is done
%% Plots and testing
% figure;
% imshow(frameY);
% title('frame Y');
% 
% figure;
% imshow(frameCr);
% title('frame Cr');
% 
% figure;
% imshow(frameCb);
% title('frame Cb');

% dummy display: downsample frameY, so that we can display the image created with the
% cat function, and just take a look on our result

% figure;
% frameY = frameY(1:2:end, 1:2:end);
% imshow(cat(3, frameY, frameCr, frameCb));
% title('YCbCr image');

end

