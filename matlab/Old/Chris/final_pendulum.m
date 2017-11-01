clc; clear all; close all;

% model: m*l^2*theta_pp + c*theta_p + m*g*l*sin(theta) = 0

data = csvread('pend_experiment.csv',1,0);
t = data(:,1); angle = data(:,2);
m = 0.2; g=9.81;
angle_0 = angle(1);
% 
% figure
% plot(t,angle);
% grid on;
% xlabel('t in s')
% legend('theta in rad')

%% Estimates angle_p by filtering angle --> get estimates l and c

% Filtered derivatives
T = t(2)-t(1);
N =1; % 2*N+1 points are used as local neighbourhood for quad fit

[angle_quad,angle_p_quad,angle_pp_quad] = quadraticSavitzkyGolay(angle',T,N);
% get rid of the NAN values of the filtered vectors (truncate vectors)
angle_quad = angle_quad(N+1:end-(N+1))';
angle_p_quad = angle_p_quad(N+1:end-(N+1))';
angle_pp_quad = angle_pp_quad(N+1:end-(N+1))';

% figure
% plot(t(N+1:end-(N+1)),angle(N+1:end-(N+1)),t(N+1:end-(N+1)),angle_quad);
% grid on
% 
% figure
% plot(t(N+1:end-(N+1)),angle_p_quad,t(N+1:end-(N+1)),angle_pp_quad);
% grid on

length = length(angle_quad);
Phi = [ -sin(angle_quad) -angle_p_quad -sign(angle_p_quad)];
theta_hat = lsqr(Phi,angle_pp_quad);
l_hat = .471;
c1_hat = 0.0004;
c2_hat = 0.004;

 sim('Pendulum_param_sim.mdl');
angle_sim = angle_sim_vec.signals.values;

figure('position', [0, 0, 1000, 1300])  % create new figure with specified size  
plot(t,angle,t,angle_sim);
grid on
legend('Measured angle','Simulation angle');