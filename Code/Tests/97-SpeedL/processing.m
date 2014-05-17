%%
close all;
clc;
load HurtigTilLangsom.txt

test01 = HurtigTilLangsom

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
