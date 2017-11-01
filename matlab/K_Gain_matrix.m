%%Script for generating K-Gain for Poles
M = 1.2;              % Mass of cart (kg)
m = 0.2;          	% Mass of pendulum (kg)
L = 0.8;            % Length of pendulum (m)
wheel_Radius = 0.088;
b = 0.01;           % Damping of cart (N.s/m) //misc, tuned
c = 0.05;           % Damping of pendulum (N.m.s/rad) //calc'd from sys id


A =    [-c/(m*L*L),  0,      c/L;
        1/(m*L*L),   0,      1/L;
        0,          0,      0];

B = [0;
     0;
     1];
 
 poles=[-5,-7,-9];
 
 K=place(A,B,poles);