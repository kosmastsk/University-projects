function [ frameRGB ] = ycrcb2ccir(  frameY, frameCr, frameCb )
%YCRCB2CCIR Reconstructs the input in RGB CCIR601 with resolution 720x576
%   2-D.8.2 in MPEG video documentation (p. 109)

disp('Hello from ycrcb2ccir function\n');
%% Initialization
% Input size:
% frameY: 360x288
%frameCr, frameCb: 180x144
% output must be of size: 720x576x3

R = double(zeros(576, 720));
G = double(zeros(576, 720));
B = double(zeros(576, 720));
frameRGB = double(zeros(576, 720, 3));

frameY = im2double(frameY);
frameCr = im2double(frameCr);
frameCb = im2double(frameCb);

lum_upsampling_weights = [-12, 0, 140, 256, 140, 0, -12];
 
chr_upsampling_weights = [1, 0, 3, 0, 3, 0, 1];

%% Vertical Upsampling Filter for chrominance SIF

% Upsample the image with offset 1 to increase the size
frameCr = upsample(frameCr, 2, 1);
frameCb = upsample(frameCb, 2, 1);

% Use interpolation to calculate the values of the zero pels that occured
% with the upsampling
for w = 1 : size(frameCr, 2)
    frameCr(:, w) = interpFilter(frameCr(:, w), chr_upsampling_weights, 8);
    frameCb(:, w) = interpFilter(frameCb(:, w), chr_upsampling_weights, 8);
end

% figure;
% imshow(frameY);
% title('frameY');
% figure;
% imshow(frameCr);
% title('frameCr');
% figure;
% imshow(frameCb);
% title('frameCb');

%% Horizontal Upsampling Filter

frameY = upsample(frameY', 2); % transpose the matrix to use the same function but upsample to the other direction
frameY = frameY'; % recover the matrix to the original state but upsampled properly

% Same procedure for chrominance channels
frameCr = upsample(frameCr', 2);
frameCb = upsample(frameCb', 2);
frameCr = frameCr';
frameCb = frameCb';

for h = 1 : size(frameY, 1)
    frameY(h, :) = interpFilter(frameY(h, :), lum_upsampling_weights, 256);
end

for h = 1 : size(frameCr, 1)
    frameCr(h, :) = interpFilter(frameCr(h, :), chr_upsampling_weights, 8);
    frameCb(h, :) = interpFilter(frameCb(h, :), chr_upsampling_weights, 8);
end

% figure;
% imshow(frameY);
% title('frameY');
% figure;
% imshow(frameCr);
% title('frameCr');
% figure;
% imshow(frameCb);
% title('frameCb');

%% Vertical Upsampling Filter

% Upsample the image with offset 1 to increase the size
frameY = upsample(frameY, 2, 1);
frameCr = upsample(frameCr, 2, 1);
frameCb = upsample(frameCb, 2, 1);

% Use interpolation to calculate the values of the zero pels that occured
% with the upsampling
for w = 1 : size(frameY, 2)
    frameY(:, w) = interpFilter(frameY(:, w), lum_upsampling_weights, 256);
end

for w = 1 : size(frameCr, 2)
    frameCr(:, w) = interpFilter(frameCr(:, w), chr_upsampling_weights, 8);
    frameCb(:, w) = interpFilter(frameCb(:, w), chr_upsampling_weights, 8);
end

% figure;
% imshow(frameY);
% title('frameY');
% figure;
% imshow(frameCr);
% title('frameCr');
% figure;
% imshow(frameCb);
% title('frameCb');

%% Convert the YCrCb to RGB

for h = 1 : size(frameY, 1) % height
    for w = 1 : size(frameY, 2) % width
        % We are using ceil(w/2) since the chromatic channels have the half
        % width comparing to frameY, so we have to use the half amount of
        % pels, but spread them to all of the luminance pels.
        R(h, w) = frameY(h, w) + 1.403 * (frameCr(h, ceil(w/2)) - 0.5);
        G(h, w) = frameY(h, w) - 0.344 * (frameCr(h, ceil(w/2)) - 0.5) - 0.714 * (frameCb(h, ceil(w/2)) - 0.5);
        B(h, w) = frameY(h, w) + 1.773 * (frameCb(h, ceil(w/2)) - 0.5);
    end
end

frameRGB(:,:,1) = R;
frameRGB(:,:,2) = G;
frameRGB(:,:,3) = B;

%% Testing and ploting - Hope it works

% figure;
% imshow(R);
% title('R');
% figure;
% imshow(G);
% title('G');
% figure;
% imshow(B);
% title('B');
% 
% figure;
% imshow(frameRGB);
% title('RGB frame reconstructed');

%% END