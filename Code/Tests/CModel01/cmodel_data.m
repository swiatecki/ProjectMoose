%% Simulation parameters for CModel01
clear all;
cd /misc/shome/ex16/_Bachelor/Code/Tests/CModel01;
% 59 Hz
%T = 0.01695;

%56 Hz
%T = 0.01786;

%60Hz
T = 1/60;

intrinsic = 779.280;
height=0.37; %37 cm
radius = 0.579106;
magic = 0.0004748;
%%

%Modify our robot model to compy with 60 hz


dPlant = c2d(tf([1],[1 0]),T) % integrator

% 2 delays @ 125 = 1 @ 60
delay = tf(1,[0 0 1 0],T)


dPlantDelay = dPlant*delay

%Husk: vi har et delay på 3(men eksperimentielt fundet at det passer bedre ved 2) - OG en integrator på 1 = z^4
margin(dPlantDelay)

%%

[num, den] = dlinmod('cmodel01_ol',T);
sys = tf(num,den,T)

margin(sys)


%60 degrees gives us -26.2 dB
format longG
db2mag(26.2)

% the 2 delay 60 degree
db2mag(21.9)

%% Test
x = Simulink.BlockDiagram.getInitialState('cmodel01');


%%
close all;
[A,B,C,D] = dlinmod('test',T,0,0.1);
sys = ss(A,B,C,D,T)

margin(sys)

%%

clc;

format shortG
delayLog = load('../camSwing/defaultLog.txt');

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % Target pos

height = tool(1,3)-0.27; % 27 cm to account for table + tool

qddActual = diff(qdActual)/T;

robotTime = delayLog(:,1);

gain = 16;

base = qActual(:,1);

%stem(base)


f = figure();
hold all
set(f,'name','Test','numbertitle','off')


%Actual poart

tv = 0:T:(length(robotTime)/(1/T))-T;

base=base*radius;
offset=base(1);
%Plot it
plot(tv,base-offset)


% Til simulink modellen
initE = -0.048 % meter
initE = -0.092863
initE = -0.0938194






sim('step_cmodel01_cl',3)




plot(simout.time,simout.signals.values*radius)
legend('"Slow" model -Actual position (base)','Simulated position');
title(sprintf('Actual Pos vs Simulated pos, Kp = %d, Step = %f',gain,initE))
xlabel('Time [s]')
ylabel('Position - relative [m]')
hold off;

%%
[num, den] = dlinmod('step_cmodel01_ol',T);
sys = tf(num,den,T)

bode(sys)

%% For cmodel 2 
% added pole

integrator = tf([1],[1 0]);
pole = tf(1,[1/20 1]);

the_sys = integrator*pole
%%
%Cmodel part 2
[num, den] = dlinmod('cmodel02_ol_2xPol',T);
sys = tf(num,den,T)

bode(sys)
grid on
db2mag(20)
%%
%still cmodel02 - template
clc;

format shortG
delayLog = load('../camSwing/defaultLog.txt');

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % Target pos

height = tool(1,3)-0.27; % 27 cm to account for table + tool

qddActual = diff(qdActual)/T;

robotTime = delayLog(:,1);

gain = 12;

base = qActual(:,1);

%stem(base)


f = figure();
hold all
set(f,'name','Test','numbertitle','off')


%Actual poart

tv = 0:T:(length(robotTime)/(1/T))-T;

base=base*radius;
offset=base(1);
%Plot it
plot(tv,base-offset)

 
% Til simulink modellen
initE = -0.048 % meter
initE = -0.092863
initE = -0.0933398






sim('step_cmodel02_cl_2xPol',3)




plot(simout.time,simout.signals.values*radius)
legend('"Slow" model -Actual position (base)','Simulated position');
title(sprintf('Actual Pos vs Simulated pos \n Added 2nd order system \n Kp = %d, Step = %f',gain,initE))
xlabel('Time [s]')
ylabel('Position - relative [m]')
hold off;

%% 
% Generates final plot, 2x pole at 