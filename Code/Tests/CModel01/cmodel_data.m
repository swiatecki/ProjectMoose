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
%height=0.37; %37 cm
radius = 0.579106;

%Til simulink modellen
% for gain = 10 initE = -0.0813736 % meter
%gain = 10;

initE = -0.0789832
gain = 12


delayLog = load('../00-picker/kp12_log.txt');
delayLog = delayLog(2:end,:);


%% 

%Modify our robot model to compy with 60 hz

dPlant = c2d(tf([1],[1 0]),T) % integrator

% 2 delays @ 125 = 1 @ 60
delay = tf(1,[0 0 1 0],T)


dPlantDelay = dPlant*delay

%Husk: vi har et delay på 3(men eksperimentielt fundet at det passer bedre
%ved 2) - OG en integrator på 1 = z^3
margin(dPlantDelay)

%% Determine Kp for

% SHIT! 
height=0.37;
[num, den] = dlinmod('cmodel01_ol',T);
sys = tf(num,den,T)

margin(sys)



format longG
% the 2 delay 60 degree
db2mag(21.9)



%%

clc;

format shortG
%delayLog = load('../camSwing/defaultLog.txt'); 

%delayLog = load('../00-picker/kp10_log.txt'); 

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % tool pos

height = tool(1,3)-0.27; % 27 cm to account for table + tool

qddActual = diff(qdActual)/T;

robotTime = delayLog(:,1);



base = qActual(:,1);

%stem(base)


f = figure();
hold all
set(f,'name','Normal model','numbertitle','off')


%Actual poart
time = (length(robotTime)/(1/T))-T;
tv = 0:T:time;

base=base*radius;
offset=base(1);
%Plot it

plot(tv,base-offset)


% Til simulink modellen


sim('step_cmodel01_cl',time)


% Compare actual VS with Cmodel01

plot(tv,simout.signals.values*radius)
legend('Actual position (base)','Simulated position');
title(sprintf('Actual Pos vs Simulated pos, Kp = %d, Step = %f',gain,initE))
xlabel('Time [s]')
ylabel('Position - relative [m]')
hold off;



%% SPACER # SPACER # SPACER # SPACER #
% 
%% For cmodel 2 --
% single pole, this goes into(manually) "step_cmodel02_cl"

integrator = tf([1],[1 0]);
pole = tf(1,[1/30 1]);

the_sys = integrator*pole
%%
%Cmodel02, 2x Pole
[num, den] = dlinmod('cmodel02_ol_2xPol',T);
sys = tf(num,den,T)

bode(sys)
grid on
db2mag(19.9)%%

%%
% 2x pole,
delay = tf(1,[1 0 0],T)
integrator = tf([1],[1 0]);
pole = tf(wn^2,[1 cwn wn^2]);

the_sys = c2d(integrator*pole,T)*delay

bode(the_sys)
db2mag(20.1)%%
%%
%still cmodel02 - template
clc;

format shortG


qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % Target pos
errorXPx = delayLog(2:end,39);
errorXm = delayLog(2:end,40);


height = tool(1,3)-0.27; % 27 cm to account for table + tool

qddActual = diff(qdActual)/T;

robotTime = delayLog(:,1);

wn = 45% 45

cwn = 2*0.25*wn % 0.30 %For delay på Z^2

base = qActual(:,1);
bTarget = qTarget(:,1);

%stem(base)


f = figure();
hold all
set(f,'name','+ 2x Pole','numbertitle','off')


%Actual poart
time = (length(robotTime)/(1/T))-T;
tv = 0:T:time;

base=base*radius;
offset=base(1);


bTarget=bTarget*radius;
Boffset=bTarget(1);

%Plot it
plot(tv,base-offset)

%Plot it
plot(tv,bTarget-Boffset)


sim('step_cmodel02_cl_2xPol',time);

% Compare actual VS to Cmodel02(2x pole)

plot(tv,simout.signals.values*radius)
legend('"Slow" model -Actual position (base)','Target','Simulated position');
title(sprintf('Closed loop. Actual Pos vs Simulated pos \n Added 2nd order system \n Kp = %d, Step = %f',gain,initE))
xlabel('Time [s]')
ylabel('Position - relative [m]')
ylim([-0.12 0.01])
hold off;

