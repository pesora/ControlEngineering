clear all
[t r y1]=Ramp2ndKI(60,0);
[t r y2]=Ramp2ndKI(60,100);
plot(t,y1,'-b',t,r,'r--')
hold on
plot(t,y2,'-k')
grid on
