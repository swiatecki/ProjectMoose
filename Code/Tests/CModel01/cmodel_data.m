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

[num, den] = dlinmod('cmodel01_ol',T,0.1);
sys = tf(num,den,T)

margin(sys)


%60 degrees gives us -94.9 dB
format longG
db2mag(90.1)

%% Test
x = Simulink.BlockDiagram.getInitialState('cmodel01');


%%
close all;
[A,B,C,D] = dlinmod('test',T,0,0.1);
sys = ss(A,B,C,D,T)

margin(sys)

step(sys)


