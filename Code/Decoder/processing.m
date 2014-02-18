%%
load test01.txt

qActual = test01(:,3:8); % Position
qdActual = test01(:,9:14); % Velocity


qdAcc = diff(qdActual); % Acceleration

plot(qdAcc(:,1)) % Base joint acc

