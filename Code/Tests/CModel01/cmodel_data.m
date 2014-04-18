%% Simulation parameters for CModel01
clear all;
cd /misc/shome/ex16/_Bachelor/Code/Tests/CModel01;
% 59 Hz
T = 0.01695;

intrinsic = 779.280;
height=0.37; %37 cm
radius = 0.60;
magic = 0.0004748;

%%

[num, den] = dlinmod('cmodel01_ol',T,0,0.1);
sys = tf(num,den,T)

margin(sys)


%60 degrees gives us -94.9 dB
format longG
db2mag(28.2)

%% Test
x = Simulink.BlockDiagram.getInitialState('cmodel01');


%%
close all;
[A,B,C,D] = dlinmod('test',T,0,0.1);
sys = ss(A,B,C,D,T)

margin(sys)

%%

clc;
format short
load nstest.txt;

delayLog = nstest;

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % Target pos

qddActual = diff(qdActual)/0.008;

robotTime = delayLog(:,1);


