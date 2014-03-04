clc;
format short
load delayLog.txt;

%delayLog = delayLog(240:270,:);

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

disp('Avg. delay, ms')
a = mean(diff(sysTime))/1000

disp('In Hz')
1/(a/1000)

%%
%Compare actual/target velcity and actual/taget acc.

f = figure()
set(f,'name','Velocity','numbertitle','off')
hold all
title('Velocity')
plot(qdTarget(:,1),'x')
plot(qdActual(:,1),'o')
plot(test01)
legend('Target vel','Actual vel')
xlim([248 266]);
ylim([-0.03 0.07])
hold off
% 
% f = figure()
% hold all
% set(f,'name','Acc','numbertitle','off')
% plot(qddTarget(:,1),'x')
% plot(qddActual(:,1),'o')
% plot(test01)
% legend('Target acc','Actual acc')
% xlim([248 266]);
% ylim([-0.6 1.5])
% hold off


%%
%Comparing velocity and acc

figure()
plot(qddActual(:,1))
figure()
plot(qdActual(:,1))
