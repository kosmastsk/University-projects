%% SCRIPT: PIPELINE_NON_LOCAL_MEANS
%
% Pipeline for non local means algorithm as described in [1].
%
% The code thus far is implemented in CPU.
%
% DEPENDENCIES
%
% [1] Antoni Buades, Bartomeu Coll, and J-M Morel. A non-local
%     algorithm for image denoising. In 2005 IEEE Computer Society
%     Conference on Computer Vision and Pattern Recognition (CVPR’05),
%      volume 2, pages 60–65. IEEE, 2005.
%

clear all %#ok
close all

%% PARAMETERS

% input image
%   pathImg = 'house64.mat';
%   pathImg   = 'house128.mat';
   pathImg   = 'house256.mat';
strImgVar = 'house';

% noise
noiseParams = {'gaussian', ...
    0,...
    0.001};

% filter sigma value
filtSigma = 0.02;
%   patchSize = [3 3]; % set the number of neighbours
%   patchSize = [5 5];
   patchSize = [7 7];

patchSigma = 5/3;

%% USEFUL FUNCTIONS

% image normalizer
normImg = @(I) (I - min(I(:))) ./ max(I(:) - min(I(:)));

%% (BEGIN)

fprintf('...begin %s...\n',mfilename);

%% INPUT DATA

fprintf('...loading input data...\n')

ioImg = matfile( pathImg );
I     = ioImg.(strImgVar);

% what is needed for the second task in report
% 
% create an array of random numbers that come from normal distribution and
% to make it easier, change each one to 0 if is smaller than 0 and normalize
% it so that the max is 1
    
%n = size(I, 2);
%m = size(I, 1);
%R = normrnd(1,1,m,n);
%R_norm = R./max(R(:));
%R_norm(R_norm<0) = 0;
%I = R_norm;

%% PREPROCESS

fprintf(' - normalizing image...\n')
I = normImg( I );

figure('Name','Original Image');
imagesc(I); axis image;
colormap gray;
title(sprintf('Original image size: %d x %d, patchSize: %d',size(I,1),size(I,2), patchSize(1)));
saveas(gcf, sprintf('01.normal size %d x %d, patchSize %d.png',size(I,1), size(I,2), patchSize(1)));

%% NOISE

fprintf(' - applying noise...\n')
J = imnoise( I, noiseParams{:} );
figure('Name','Noisy-Input Image');
imagesc(J); axis image;
colormap gray;
title(sprintf('Noisy-Input Image size: %d x %d, patchSize: %d',size(J,1),size(J,2), patchSize(1)));
saveas(gcf, sprintf('02.noise size %d x %d, patchSize %d.png',size(J,1), size(J,2), patchSize(1)));

%% NON LOCAL MEANS

tic;
If = nlm( J, patchSize, filtSigma, patchSigma );
toc
isequal(I, If)
%% VISUALIZE RESULT

figure('Name', 'Filtered image');
imagesc(If); axis image;
colormap gray;
title(sprintf('Filtered image size: %d x %d, patchSize: %d',size(If,1),size(If,2), patchSize(1)));
saveas(gcf, sprintf('03.nlm size %d x %d, patchSize %d.png',size(If,1), size(If,2), patchSize(1)));

figure('Name', 'Residual');
imagesc(If-J); axis image;
colormap gray;
title(sprintf('Residual size: %d x %d, patchSize: %d',size(If-J,1),size(If-J,2), patchSize(1)));
saveas(gcf, sprintf('04.residual size %d x %d, patchSize %d.png',size(If-J,1), size(If-J,2), patchSize(1)));

%% (END)

fprintf('...end %s...\n',mfilename);


%%------------------------------------------------------------
%
% AUTHORS
%
%   Dimitris Floros                         fcdimitr@auth.gr
%
% VERSION
%
%   0.1 - December 28, 2016
%
% CHANGELOG
%
%   0.1 (Dec 28, 2016) - Dimitris
%       * initial implementation
%
% ------------------------------------------------------------
