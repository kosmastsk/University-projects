%% SCRIPT: SAMPLE_KERNEL
function If = nonLocalMeans(I, patchSize, filtSigma, patchSigma)
%
% Sample usage of GPU kernel through MATLAB
%
% DEPENDENCIES
%
%  sampleAddKernel.cu
%  Kernel2.cu
%
  
  %% PARAMETERS
  
  threadsPerBlock = [8 8];
  m = 64;
  n = 64;
  
%   threadsPerBlock = [8 16];
%   m = 128;
%   n = 128;
%   
%   threadsPerBlock = [16 16];
%   m = 256;
%   n = 256;

  %% (BEGIN)

  fprintf('...begin %s...\n',mfilename);  
  
  % KERNEL
  
  k = parallel.gpu.CUDAKernel( 'sampleAddKernel.ptx', ...
                               'sampleAddKernel.cu');
  
  numberOfBlocks  = ceil( [m n] ./ threadsPerBlock );
  
  k.ThreadBlockSize = threadsPerBlock;
  k.GridSize        = numberOfBlocks;
  
  %% DATA
  
  A1 = padarray(I, (patchSize-1)./2, 'symmetric');
  sizeIm = size(I);
  A2 = zeros(prod(sizeIm),prod(patchSize)); %store neighbors
  
  H = fspecial('gaussian',patchSize, patchSigma); %filter to apply
  H = H(:) ./ max(H(:));
  H = H';
  
  A = gpuArray(A1);
  B = gpuArray(A2);
  
  B = gather( feval(k, A, B, H, m, n, patchSize(1)) );
  
   %% SECOND KERNEL
  k2 = parallel.gpu.CUDAKernel( 'Kernel2.ptx', ...
                               'Kernel2.cu');
                             
  k2.ThreadBlockSize = threadsPerBlock;
  k2.GridSize        = numberOfBlocks;

  If = reshape(I, [m*n, 1]);
  
  If = gather( feval(k2, If, B, m, n, patchSize(1), filtSigma));
  If = reshape(If, [m n]);
  
  
  %% INSANITY CHECK
  
  %fprintf('Error: %e\n', norm( B - (A+1), 'fro' ) );
  
  %% (END)

  fprintf('...end %s...\n',mfilename);

  end
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
