%%
% Evaluating actual controller

clc;
format short
%load log.txt;
load log_kp18_step03.txt
load delayLogDefault.txt	

delayLog = log_kp18_step03;
delayLog = delayLogDefault;

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
test01 = delayLog(:,33);


qddActual = diff(qdActual)/0.008;

robotTime = delayLog(:,1);

%Systime
sysTime = delayLog(:,2);

%%

f = figure()
hold all
set(f,'name','Actual position vs simulated','numbertitle','off')
tv = 0:0.008:(length(robotTime)/125)-0.008;
plot(tv,qActual(:,1)+offset)



% Til simulink modellen
initE = 0.448025


sim('model02',2)
offset=qActual(1,1);


plot(simout.time,simout.signals.values)
legend('Actual position','Simulated position');
hold off;
