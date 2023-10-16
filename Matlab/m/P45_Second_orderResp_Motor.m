Kp=input("Proportional Gain(Kp)=");
K=1;
Tconst=0.024;
omegan=sqrt(Kp*K/Tconst);
cheta=1/(2*Tconst*omegan);
omegad=sqrt(1-cheta^2)*omegan;
t=[0:0.001:0.3];
for i=1:1:301,
    r(i)=1;
    x(i)=1-exp(-cheta*omegan*t(i))*(cos(omegad*t(i))+cheta*sin(omegad*t(i))/sqrt(1-cheta^2));
end
plot(t,x,'b',t,r,'--r')
grid on
