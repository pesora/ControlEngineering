cheta=0.5;omegan=50;
omegad=omegan*sqrt(1-cheta^2)
pi=atan(1)*4;
Mp=exp(-cheta*pi/sqrt(1-cheta^2))*100
tp=pi/omegad
ts=log(0.02)/(-cheta*omegan)
t=[0:0.0001:0.3];
num=[omegan^2]
den=[1 2*cheta*omegan omegan^2];
x=step(num,den,t);
[xmax i]=max(x)
Mp=(xmax-1)/1*100;
tp=t(i)
plot(t,x,'b')
grid on
title('Unit-Step Response of G(s)=wn*wn/S^2+2cheta*wnS+wn^2')
xlabel('Time(sec)')
ylabel('Position')


