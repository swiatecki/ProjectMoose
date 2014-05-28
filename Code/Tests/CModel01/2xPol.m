%% 2xpol OL!!!! checking if the velocity has a 2x pole

% HUSK AT VI SAMMENLIGNER VELCOTY !!
%still cmodel02 - template
clc;
%clear all;
format shortG
%close all;

% OBS OBS OBS !! RAD in this one
initE = -1;
T_actual = 1/125
%T_sim = 1/60;
radius = 0.579106;
kp=1;

delayLog = load('../125-OLTest/hah.txt');

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



%base = qActual(:,1);
vel = qdActual(:,1);

vel= vel(1:50);



%stem(base)
figure()
a=qddTarget(:,1)
a=a(1:50)
x=length(a)
plot(a)

figure()
targetV=qdTarget(:,1)
targetV=targetV(1:50)
x=length(a)




f = figure();
hold all
set(f,'name','OpenLoop+ 2x Pole','numbertitle','off')

%Actual poart
time = 2;
tv = 0:T:time;

%base=base*radius;
offset=vel(1);
%Plot it
x=length(vel)
plot(vel-offset,'bo')


plot(targetV,'ro')


wn = 20% 45 %DIS ONE!
xi = 0.4;
cwn = 2*xi*wn % 0.30 %For delay på Z^2
sim('step_cmodel02_ol_2xPol',time-1);
x=length(olSim.signals.values);
% Compare actual VS to Cmodel02(2x pole)

%plot(tv(1:x),olSim.signals.values,'rx')

title(sprintf('Open Loop Actual Veloity vs Simulated velocity \n Added 2nd order system \n Kp = %d, Step = %f',kp,initE))
xlabel('Time [s]')
ylabel('Velocity -  [rad/s]')

legend('Actual',sprintf('wn=%d,C=%d',wn,xi))






hold off;