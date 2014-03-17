clc;
close all;

% Controller #1 for Model01.


%Init the model. This gives us the "dPlantDelay"
run('../model01.m');




margin(dPlantDelay)
%%
Kp = db2mag(25.2)

controller = dPlantDelay*Kp
margin(controller)


sys = feedback(controller,1)




figure()
step(sys)