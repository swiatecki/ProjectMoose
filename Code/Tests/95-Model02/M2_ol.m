clear all;
clc
close all;

T = 1/125;

Kp =1;
close all;
[A,B,C,D] = dlinmod('model02_ol',T,0,1);
sys = ss(A,B,C,D,T);
%%
close all;
bf = figure();
b = bodeplot(sys)

grid on


p = getoptions(b);
%p.FreqUnits = 'Hz' ;
p.Xlabel.FontSize = 12;
p.Ylabel.FontSize = 12;
p.Title.FontSize = 12;
p.TickLabel.FontSize = 12;
setoptions(b,p)

%set(gcf,'paperunits','centimeters','Paperposition',[0 0 14 8])
saveas(gcf,'export/model02bode.eps','epsc')


%h=get(gca,'Title')
%t=get(h,'String') %t is now 'Sin(x)'
%new_t=strcat(t,' Sine function')
%title(new_t)
%title('Test')

Kp_new = db2mag(25.5) % manually found at 60 deg.

mf = figure()
set(gca,'fontsize',14); % Font size for labels etc.
margin(sys), grid




%% 
% Export simulink:
 % Print -> file, portrait, papersize!
 
 % device properties: -depsc
