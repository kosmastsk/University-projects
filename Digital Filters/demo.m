% Task 4B - Neural Network training with backpropagation
% Kosmas Tsiakas - 8255
% Digital Filter 2018
clear all;
close all;

fprintf('\n *** begin %s ***\n\n', mfilename);

%% Load data
[x, y] = iris_dataset;
% x : irisInputs is a 4x150 matrix of four attributes of 1000 flowers
% y : irisTargets is a 3x150 matrix of 1000 associated class vectors

% Given in the task
lambda = 0.8;
b = 1;
h = @(x) (1 ./ (1 + exp(-b * x))); % sigmoid activation function
dh = @(x) (h(x) .* (1 - h(x))); % first derivative of the sigmoid function

%% Separate testing and training data with a random way (half - half)
index = randperm(length(x));

totalNum = length(x);
trainNum = totalNum/2;

train_x = x(:, index(1:trainNum));
train_y = y(:, index(1:trainNum));

test_x = x(:, index(trainNum+1 : end));
test_y = y(:, index(trainNum+1 : end));

%% Define constants related to the NN and the training
epochs = 2000;
inputSize = size(x,1);
outputSize = size(y,1);

layers = [inputSize; 6; 4; outputSize];
layersNum = length(layers);

u = cell(1,layersNum);
v = cell(1,layersNum-1);

for i = 1 : layersNum
    u{i} = zeros(layers(i), 1);
end

for i = 2 : layersNum
    v{i-1} = zeros(layers(i), 1);
end

delta = cell(1, layersNum-1);
for l = 2 : layersNum
	delta{l-1} = zeros(layers(l), 1);
end

%% Initialize the weights with small numbers
weights = cell(1, layersNum-1);

for i = 1 : layersNum-1
    weights{i} = rand(layers(i+1), layers(i));
end

%% Start training
J = zeros(epochs, 1);
for i = 1 : epochs
    gradit = cell(1, layersNum-1);
    for n = 1 : layersNum-1
        gradit{n} = zeros(size(weights{n}));
    end

    %% Forward path
    for j = 1 : trainNum
        u{1} = train_x(:, j);
        for l = 1 : layersNum-1
            v{l} = weights{l} * u{l};
            u{l+1} = h(v{l});
        end
        output = u{layersNum};
       
        error = train_y(:, j) - output;
        Jt = immse(output,train_y(:, j)); % mean square error

        J(i) = J(i) + Jt;
        
        %% Backward path
        delta{layersNum-1} = error .* dh(v{layersNum-1});
        
        gradit{layersNum-1} = gradit{layersNum-1} - delta{layersNum-1} * u{layersNum-1}';
        
        for l = layersNum-2 : -1 : 1
            delta{l} = dh(v{l}) .* (weights{l+1}' * delta{l+1});
            gradit{l} = gradit{l} - delta{l} * u{l}';
        end
                        
    end
    %% Modify the weights
    for l = 1 : layersNum-1
        weights{l} = weights{l} - lambda * gradit{l} / (100*norm(gradit{l}));
    end

end

figure();
plot(J./trainNum);
title('Mean Square Error on output of each epoch (accumulated error / trainNum)');

%% Validation
testError = zeros(1, totalNum - trainNum);

for i = 1 : totalNum - trainNum
    u{1} = test_x(:, i);    
    for l = 1 : layersNum-1
        v{l} = weights{l}* u{l};
        u{l+1} = h(v{l});
    end
    
    output = u{layersNum};

    error = test_y(:, i) - round(output);

    if any(error) ~= 0
        testError(i) = 1;
    end

end

figure;
stem(testError(:));
title('Wrong output');