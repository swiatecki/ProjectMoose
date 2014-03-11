clc;
clear all;

T = 0.008; % 8 ms

dPlant = c2d(tf([1],[1 0]),T)


delay = tf(1,[1 0 0 0],T)


dPlantDelay = dPlant*delay

%Husk: vi har et delay på 3 - OG en integrator på 1 = z^4


%%
%This plot should only by used for display. Actual use should be scaled.
%See trackDelayNetwork/matlab.m for use


%Below is commented out, so it wont display a image when the script is
%called from another file.

%figure()
%step(dPlantDelay,0.2)
%title('Step response of simple model')





