K=1;
Tconst=0.024;
t=[0:0.001:0.1];
for i=1:1:101,
    r(i)=1;
end
num=[K];
den=[Tconst 1];
x=step(num,den,t);
plot(t,x,'b',t,r,'--r');
grid on
axis([0 0.1 -0.1 1.2])
title('Unit-Step Response of G(s)=K/(Ts+1)')
xlabel('Time(sec)')
ylabel('Velocity')
