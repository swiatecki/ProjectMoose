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

%% Compare actual/target velcity and actual/taget acc.
% GENERATES: step01.eps

f = figure()
set(f,'name','Velocity','numbertitle','off')
hold all
title('\bf Velocity')
plot(qdTarget(:,1),'*b','MarkerSize',10)
plot(qdActual(:,1),'or','MarkerSize',10)
stairs(test01,'g-.','LineWidth',2)
legend('Target velocity','Actual velocity','Step')
xlim([248 255]);
ylim([-0.01 0.15])
xlabel('Sample no.');
ylabel('Velocity (rad/s)');
set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'../../step01.eps','psc2')
hold off


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

f = figure()
set(f,'name','Position','numbertitle','off')
hold all
stem(qTarget(:,1))
plot(test01*1.09)
hold off


%% Testing our model of the robot

%Plot position and model.. 
% Might want to run ../Model01/model01.m first
stepsize = 0.4; %rad/s
offset = 1.091; %Offset in actual position



f = figure()
set(f,'name','Position','numbertitle','off')
subplot(1,2,1);
hold all
title('Target Position vs Model position')
plot(qTarget(:,1)-offset,'*b','MarkerSize',5)

%opt = stepDataOptions('InputOffset',1000);
s = step(dPlantDelay,3);
svector = [zeros(1,250) s'];
svector = svector*0.4;
plot(svector ,'rx')

%plot(test01)
legend('Target position','Model position')
xlim([248 550]);
ylim([-0.03 1])
xlabel('Sample no.');
ylabel('Relative position [rad]')
hold off


subplot(1,2,2);
hold all
title('Close up')
plot(qTarget(:,1)-offset,'*')

%opt = stepDataOptions('InputOffset',1000);
s = step(dPlantDelay,3);
svector = [zeros(1,250) s'];
svector = svector*0.4;
plot(svector ,'rx')

stairs(test01*0.02,'g-.','LineWidth',2)
legend('Target position','Model position','Step')
xlim([249 265]);
%ylim([-0.03 1])
xlabel('Sample no.');
ylabel('Relative position [rad]')
hold off

set(gcf,'paperunits','centimeters','Paperposition',[0 0 16 10])
saveas(gcf,'../../model01compare.eps','psc2')