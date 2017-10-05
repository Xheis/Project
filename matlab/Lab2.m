%[Time,V,F] = xlsread("cart_experiencement.csv")
clear;

filename = 'cart_experiment.csv';
[num,txt,raw] = xlsread(filename);
%out = 
T = num(:,1);
v  = num(:,2);
N  = num(:,3);
TimeStep = 0.01;
SmoothingFactor = 3;

tf2 = T;
ft = N;

[y,dy,ddy] = quadraticSavitzkyGolay(v',TimeStep,SmoothingFactor); %N is the smoothing factor

v = v(1:end-3);
y = y(1:end-3);
dy = dy(1:end-3);
ddy = ddy(1:end-3);
T = T(1:end-3);
N = N(1:end-3);
v = v(4:end);
y = y(4:end);
dy = dy(4:end);
ddy = ddy(4:end);
T = T(4:end);
N = N(4:end);
size(ddy)
size(dy)
size(T)
size(N)

P = lsqr([dy' y'],N);

M = P(1);
B = P(2);
b = B;
%sim('simulated_lab2_1');
sim('ver_sim2');
y_estimated = vel_sim;
t_estimated = time_sim
figure(1);clf
plot(T,v,t_estimated,y_estimated,'LineWidth',2)
xlim([0 T(end)]);
xlabel('Time (sec)')
title('Original signal (blue) VS Estimated Signal (orange)')
grid on
