clear all
close all

N = [2^20, 2^21, 2^22, 2^23, 2^24, 2^25];
t = [0.0639, 0.1329, 0.2944, 0.5851, 1.2398, 2.6149];

figure
scatter(N, t)
title('Total Execution Time')
xlabel('Number of particles')
ylabel('Execution Time')
xlim([2^20 2^25])
ylim auto;