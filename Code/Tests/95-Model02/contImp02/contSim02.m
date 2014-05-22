%%
% Evaluating actual controller
clear all;
clc;
format short
%load log.txt;
%load log_kp18_step03.txt
%load kp12.txt
%load kp11.txt
%load delayLogDefault.txt	

T = 1/125;


delayLog = load('delayLogDefault.txt');

%delayLog = kp12;
%delayLog = delayLogDefault;

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
saturation = delayLog(:,33);


qddActual = diff(qdActual)/0.008;

robotTime = delayLog(:,1);

%Systime
sysTime = delayLog(:,2);
initE = 0.691674
Kp = 15;
%%

f = figure()
hold all
offset=qActual(1,1);
set(f,'name','Actual position vs simulated','numbertitle','off')

time = (length(robotTime)/125)-0.008

corrected = qActual(:,1)-offset;

tv = 0:0.008:time;
plot(tv,corrected,'linewidth',2);



% Til simulink modellen





sim('model02',time)



xlabel('Time [s]');
ylabel('Relative Position [rad]');

%plot(simout.time,simout.signals.values)
plot(tv,simout.signals.values,'linewidth',2)
xlim([0 1.6]);
ylim([-0.1 1])
%plot(tv,saturation)
set(gca,'fontsize',14); % Font size for labels etc.
title({'3 sample delay','Actual position vs Simulated position ',sprintf('Kp = %d, Step = %f',Kp,initE)})
legend('Actual','Simulated');
set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'export/k15s3.eps','epsc')

hold off;


%%
% MODEL 2 - QUICK !!!

f = figure()
hold all
set(f,'name','Actual position vs simulated Quick!!','numbertitle','off')

time = (length(robotTime)/125)-0.008;
tv = 0:0.008:time;

corrected = qActual(:,1)-offset;

plot(tv,corrected,'linewidth',2)



% Til simulink modellen

xlabel('Time [s]');
ylabel('Relative Position [rad]');

sim('model02_quick',time)
offset=qActual(1,1);
xlim([0 1.6]);
ylim([-0.1 1])

set(gca,'fontsize',14); % Font size for labels etc.
plot(tv,simout.signals.values,'linewidth',2)
title({'2 sample delay','Actual position vs simulated position ',sprintf('Kp = %d, Step = %f',Kp,initE)})
legend('Actual','Simulated');
set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'export/k15s2.eps','epsc')
hold off;
