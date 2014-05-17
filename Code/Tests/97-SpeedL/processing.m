%% REPORT! - acc.eps
close all;
clc;
test = load('accelerate_.txt');

startAt = 200;

qActual = test(startAt:end,3:8); % Actual Position
qdActual = test(startAt:end,9:14); %Actual  Velocity
qddTarget = test(startAt:end,15:20); % Target acc



fs=125;

a = qddTarget(:,1);

figure()
%[x y] = stairs(qddTarget(:,1))
t = 0:1/fs:(length(a)/125)-1/fs;
stairs(t,a-a(1),'LineWidth',2)
title({'\bf Slow to fast transition. \Deltav = 0.3 [m/s]', 'Target acceleration (Base joint)'},'FontSize',16);
set(gca,'fontsize',14); % Font size for labels etc.
xlabel('Time [s]');
xlim([0 2.5])
ylabel('Joint acceleration [rad/s^2]');
set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'export/acc.eps','epsc')

%% REPORT! - dec.eps
clear all;
close all;
clc;
test = load('decelerate_.txt');

startAt = 200;

qActual = test(startAt:end,3:8); % Actual Position
qdActual = test(startAt:end,9:14); %Actual  Velocity
qddTarget = test(startAt:end,15:20); % Target acc



fs=125;

a = qddTarget(:,1);

figure()
%[x y] = stairs(qddTarget(:,1))
t = 0:1/fs:(length(a)/125)-1/fs;
plot(t,a-a(1),'LineWidth',2)
title({'\bf Fast to slow transition. \Deltav > 0.1 [m/s]', 'Target acceleration (Base joint)'},'FontSize',16);
set(gca,'fontsize',14); % Font size for labels etc.
xlim([0 1.7])
xlabel('Time [s]');
ylabel('Joint acceleration [rad/s^2]');
set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'export/dec.eps','epsc')

%% REPORT! - dec-small.eps
clear all;
close all;
clc;
test = load('decelerate_small_step.txt');

startAt = 200;

qActual = test(startAt:end,3:8); % Actual Position
qdActual = test(startAt:end,9:14); %Actual  Velocity
qddTarget = test(startAt:end,15:20); % Target acc



fs=125;

a = qddTarget(:,1);

figure()
%[x y] = stairs(qddTarget(:,1))
t = 0:1/fs:(length(a)/125)-1/fs;
plot(t,a-a(1),'LineWidth',2)
title({'\bf Fast to slow transition. \Deltav \leq 0.1 [m/s]', 'Target acceleration (Base joint)'},'FontSize',16);
set(gca,'fontsize',14); % Font size for labels etc.
xlabel('Time [s]');
xlim([0 1.7])
ylabel('Joint acceleration [rad/s^2]');
set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'export/dec-small.eps','epsc')
%%
%%
clc;
load LangsomTilHurtig.txt

test02 = LangsomTilHurtig

qActual = test02(230:end,3:8); % Actual Position
qdActual = test02(230:end,9:14); %Actual  Velocity
qddTarget = test02(230:end,15:20); % Target acc


qdAcc = diff(qdActual); % Acceleration

%plot(qdAcc(:,1)) % Base joint acc



figure()
plot(qddTarget(:,1))
title('2 Target Acc, joint 1');

figure()
plot(qdActual(:,1))
title('2 Actual velocity, joint 1');

figure()
plot(qActual(:,1))
title('2 Actual Position, joint 1');

%%

close all;
clc;
load test02.txt;

test01 = test02;

qActual = test01(230:end,3:8); % Actual Position
qdActual = test01(230:end,9:14); %Actual  Velocity
qddTarget = test01(230:end,15:20); % Target acc


qdAcc = diff(qdActual)/0.008; % Acceleration

%plot(qdAcc(:,1)) % Base joint acc



figure()
plot(qddTarget(:,1))
title('Target Acc, joint 1');

figure()
plot(qdAcc(:,1))
title('Actual Acc, joint 1');

figure()
plot(qdActual(:,1))
title('Actual velocity, joint 1');

figure()
plot(qActual(:,1))
title('Actual Position, joint 1');
