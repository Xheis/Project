%[Time,V,F] = xlsread("cart_experiencement.csv")
clc; clear;

filename = 'pend_experiment.csv';
[num,txt,raw] = xlsread(filename);
%out = 
T = num(:,1);
theta_0  = num(:,2);
TimeStep = 0.01;
N = 16;

%?,?',?''
[y,dy,ddy] = quadraticSavitzkyGolay(theta_0',TimeStep,N); %N is the smoothing factor

% theta_0 = theta_0(1:end-3);
% y = y(1:end-3);
% dy = dy(1:end-3);
% ddy = ddy(1:end-3);
% T = T(1:end-3);
% theta_0 = theta_0(4:end);
% y = y(4:end);
% dy = dy(4:end);
% ddy = ddy(4:end);
% T = T(4:end);
% size(ddy)
% size(dy)
% size(T)

%sin0 = []
% lsqrB = [sin(y(N+1:end-N))];
% lsqrA = [ddy'(N+1:end-N) dy'(N+1:end-N)];

m = 0.2; 
g = 9.8;
%P = lsqr(lsqrA,lsqrB);
P = lsqr([ddy(N+1:end-N)' dy(N+1:end-N)'],-sin(y(N+1:end-N)'));
l = P(1)*g;
c = P(2)*m*g*l;
% L = 0.3
% l = L
% c = 0.025


sim('lab_2_chris');
figure(1);clf
plot(T,theta_0,time_sim,theta_sim,'LineWidth',2)
xlim([0 T(end)]);
xlabel('Time (sec)')
title('Original signal (blue) VS Estimated Signal (orange)')
grid on
