%% State Space Calcs
%
syms ddx ddO dx dO x O F
syms M m L g b c
% M = 1;
% m = 1;
% L = 1;
% g = -9.8;
% b = 1;
% c = 1; 

Z = [dx; dO; x; O]
dZ = [ddx; ddO; dx; dO]

%
%dZ = [? ? ? ?;     [dx      [? 
%      ? ? ? ?;      dO       ? 
%      1 0 0 0;   *  x    *   0   * F
%      0 1 0 0]      O]       0] 

% [dZ] =  ( [F; 0] - B[Z] - C[x; O] ) * A^-1
A = [M+m m*L; m*L m*L*L]
B = [b 0; 0 c]
C = [0 0; 0 -L*m*g]
D = [F; 0]
F = [dx; dO]
E = [x; O]

%ans = (D*inv(A) - B*F*inv(A) - C*E*inv(A))
temp1 = (1/(M+m));
temp2 = (-b/(M+m));
ans1 = [temp1; 0; temp2; 0]%forcing this %D*inv(A)
ans2 = -B*inv(A)
ans3 = -C*inv(A)
%So we need [ddx; ddO]

%% Now we have values
%dZ = [A1 A2 B1 B2;     [dx      [? 
%      A3 A4 B3 B4;      dO       ? 
%      1  0  0  0;   *   x    *   0   * F
%      0  1  0  0]       O]       0] 

%% Sub in values
%dZ = [0 0 B1 B2;     [dx      [? 
%      -(g*m)/M A4 B3 B4;      dO       ? 
%      1  0  0  0;   *   x    *   0   * F
%      0  1  0  0]       O]       0] 

fA = [ans2(1,1) ans2(1,2) ans3(1,1) ans3(1,2);
      ans2(2,1) ans2(2,2) ans3(2,1) ans3(2,2)
      1  0  0  0;
      0  1  0  0]


fB = [ans1(1,1) ; 0; ans1(3,1); 0;]
fC = [0 1 0 0]


%% Theory Again
% Z =  dx
%      dO
%       x
%       O

% dZ = Az + Bu
% y = cx + du

% dZ = fA*Z + fB*F
% y = fC*dx

% Fuck to the yes.
% That is essentially
% dx(t) = Ax(t) + Bu(t)
% y(t) = Cx(t),

% Now we need to apply the "control law"
% "u(t) = Nr(t) ? Kx(t)"
% "F = N*rF - KZ"

% Z = (A ? B*K)Z + B*N*rF
% y = C*Z

%% Now we can use matlab to find a K to control this

% What poles do we want?
% Well, good poles are one that ...

desiredPoles = [-5.0 -6.0 -6.5 -7.0];

M = 1;
m = 1;
L = 1;
g = -9.8;
b = 1;
c = 1; 

dummyA = [0,0,0,0;
    0,0,0,0;
    0,0,0,0;
    0,0,0,0;]
% 
% testA = [    -1/1,                1/(1*1),        0,                 0;
%           1/(1*1), -(1*(1 + 1))/(1^2*1*1), -(1*1)/1, (1*(1 + 1))/(1*1);
%                 1,                      0,        0,                 0;
%                 0,                      1,        0,                 0]
% testB = [ 1/(1 + 1);
%              0;
%          -1/(1 + 1);
%              0;]

            
%K = place(testA,testB,desiredPoles)
K = place(fA,fB,desiredPoles)
