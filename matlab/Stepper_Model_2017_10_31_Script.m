clc
clear all
close all;

%% Variables
% Cart/pendulum parameters
M = 1.2;              % Mass of cart (kg)
m = 0.2;          	% Mass of pendulum (kg)
L = 1.06;            % Length of pendulum (m)
wheel_Radius = 0.088;
b = 0.01;           % Damping of cart (N.s/m) //misc, tuned
c = 0.017;           % Damping of pendulum (N.m.s/rad) //calc'd from sys id

% Other parameters
startAngle = 5;
g = 9.82;           % Acceleration due to gravity (m/s/s)
%alpha = 1*pi/180;   % Ground slope (rad = deg*pi/180)
theta0 = startAngle*pi/180;  % Initial angle (rad = deg*pi/180)
dV0 = 0.01;


Velocity_Max = 0.035;
Velocity_Min = -0.035;
%% Signal Generation

Velocity_Step = 0.1;
%% Calculating K-Gain
% A =    [-c/(m*L*L),  0,      c/L;
%         1/(m*L*L),   0,      1/L;
%         0,          0,      0];
%Original Matrixes-----------------
A =    [-c/(m*L*L),  m*g*L, c/L;
        1/(m*L*L),   0,     -1/L;
        0,          0,      0];

    
B = [0;
     0;
     1];
 
 C = [0 0 1];
 
 D = 0;
%----------------------------------

poles=[-1,-3,-5]; %[-5,-6,-9];

K=place(A,B,poles);
N= 1/((C-D*K)/(B*K-A)*B+D);
%A_new = A-B*K;

Controller_Gain = K(3);
%%  
%sym theta v x omega


% -----------------------------------
% Inner (angle) controller parameters
%zeroes = 5.4277;%5.4277;
%poles =  25.4277;%25.4277;
%Controller_Gain = -119.0071;%-119.0071;
% -----------------------------------

% Outer (velocity) controller parameters
% TF_Theta_To_V_Gain = 1;
% TF_Theta_To_X_Gain = 1;


% Run simulation
tsim = 60;
sim('Stepper_Model_2017_10_31'); %_mark');


% Plot results
figure(1);clf
subplot(2,2,1)
%plot(t,theta,Plant_Real(:,1),Plant_Real(:,2)*pi/90,'r')
plot(t,theta,t,ri,'r')
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('\theta (\circ)')
legend('Actual Plant','Demanded','Location','Best')

subplot(2,2,2)
plot(t,omega*180/pi)
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('\omega (\circ/s)')

subplot(2,2,3)
plot(t,x)
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('x (m)')

subplot(2,2,4)
plot(t,V,t,r,'r')
grid on
xlim([0 tsim])
xlabel('Time (sec)')
ylabel('v REF (m/s)')
legend('Actual','Demanded','Location','Best')

figure(2);clf
set(gcf,'color','w')
%import my data
% Plant_Real = csvread('Real_Plant_Pend_Swing_Test.csv');
% plot(t,theta,Plant_Real(:,1),Plant_Real(:,2)*pi/90,'r');
% grid on
% title('Real vs Plant')
% xlabel('Time (sec)')
% ylabel('Angle (Degrees)')
% legend('Simulation, c = 0.017 N.m.s/rad','Actual Plant')

plot(t,u)
grid on
title('Control force (Velocity)')
xlabel('Time (sec)')
ylabel('Velocity (m/s)')