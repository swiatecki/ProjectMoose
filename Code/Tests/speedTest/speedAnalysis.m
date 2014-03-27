load speedLog.txt

s = speedLog(:,1);


qActual = speedLog(:,3:8); % Actual Position
qdActual = speedLog(:,9:14); %Actual  Velocity
qddTarget = speedLog(:,15:20); % Target acc
qdTarget = speedLog(:,21:26); % Target vel
qTarget = speedLog(:,27:32); % Target pos
test01 = speedLog(:,33);


qddActual = diff(qdActual)/0.008;

robotTime = speedLog(:,1);

%Systime
sysTime = speedLog(:,2);

tv = 0:0.008:(length(robotTime)/125)-0.008;

%%
plot(qdActual(:,1))


%%
x = qActual(61:419,1);
tvx = 0:0.008:((419-61)/125);
plot(tvx,x)


(0.6701+1.834)/2.5