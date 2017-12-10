%first plot for 64x64 image
times64 = [0.869702 0.152821 0.900850 0.175433 0.939229 0.218249];
type = {'CPU:3', 'GPU:3', 'CPU:5', 'GPU:5', 'CPU:7', 'GPU:7'};
figure;
bar(times64);
title('image size: 64x64');
xlabel('CPU or GPU : patchSize');
ylabel('execution time in seconds');
set(gca, 'XTick', 1:6);
set(gca, 'XTickLabel', type);

%second plot for 128x128 image
times128 = [12.815585 0.373953 13.182833 0.856707 14.012289 1.687486];
type = {'CPU:3', 'GPU:3', 'CPU:5', 'GPU:5', 'CPU:7', 'GPU:7'};
figure;
bar(times128);
title('image size: 128x128');
xlabel('CPU or GPU : patchSize');
ylabel('execution time in seconds');
set(gca, 'XTick', 1:6);
set(gca, 'XTickLabel', type);

%third plot for 256x256 image
times256 = [Inf 3.361179 Inf 8.465803 Inf 16.909172];
type = {'CPU:3', 'GPU:3', 'CPU:5', 'GPU:5', 'CPU:7', 'GPU:7'};
figure;
bar(times256);
title('image size: 256x256');
xlabel('CPU or GPU : patchSize');
ylabel('execution time in seconds');
set(gca, 'XTick', 1:6);
set(gca, 'XTickLabel', type);

