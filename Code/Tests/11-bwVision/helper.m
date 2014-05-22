clc;
format short
load movelog

delayLog = movelog.txt;

qActual = delayLog(:,3:8); % Actual Position
qdActual = delayLog(:,9:14); %Actual  Velocity
qddTarget = delayLog(:,15:20); % Target acc
qdTarget = delayLog(:,21:26); % Target vel
qTarget = delayLog(:,27:32); % Target pos
tool = delayLog(:,33:38); % Target pos

qddActual = diff(qdActual)/0.008;

robotTime = delayLog(:,1);
