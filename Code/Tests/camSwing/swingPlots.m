clc;
format shortG
delayLog = load('saveLogs/swing1_5rads.txt');

delayLog = load('saveLogs/swing1_2rads.txt');

delayLog = load('saveLogs/swing0_2rads.txt');
delayLog = load('saveLogs/swing1rads.txt');


qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % Target pos

qddActual = diff(qdActual)/0.008;

robotTime = delayLog(:,1);

base = qActual(:,1);

distPx = delayLog(:,39);
distm = delayLog(:,40);

%%
figure()
plot(base,distPx)
ylim([-500 500])