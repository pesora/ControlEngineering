K=1;
Tconst=0.024;
t=[0:0.001:0.2];
for i=1:1:201,
    r(i)=t(i);
end
num=[K];
den=[Tconst 1 0];
x=step(num,den,t);
plot(t,x,'b',t,r,'--r');
grid on
%axis([0 0.1 -0.1 1.2])
title('Unit-Ramp Response of G(s)=K/(Ts+1)')
xlabel('Time(sec)')
ylabel('Velocity')
