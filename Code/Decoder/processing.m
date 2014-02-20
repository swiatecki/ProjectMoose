%%
load test01.txt

qActual = test01(:,3:8); % Actual Position
qdActual = test01(:,9:14); %Actual  Velocity
qddTarget = test01(:,15:20); % Target acc


qdAcc = diff(qdActual); % Acceleration

%plot(qdAcc(:,1)) % Base joint acc

plot(qddTarget(:,1))
