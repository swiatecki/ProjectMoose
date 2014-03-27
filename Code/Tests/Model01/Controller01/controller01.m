clc;


% Controller #1 for Model01.


%Init the model. This gives us the "dPlantDelay"
run('../model01.m');




margin(dPlantDelay)
%% Desgin a controller with 60 degrees phase margin
Kp = db2mag(25.2)

controller = dPlantDelay*Kp
margin(controller)


sys = feedback(controller,1)




figure()
step(sys)

