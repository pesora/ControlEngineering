clear all
[t r y1]=Step2ndPID(60,0,0);
[t r y2]=Step2ndPID(60,0,1);
hold off
plot(t,y1,'-b',t,r,'--r')
hold on
plot(t,y2,'-g')
grid on
