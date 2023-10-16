K=1;
Tconst=0.024;
t=[0:0.001:0.1];
for i=1:1:101,
    r(i)=1;
    x(i)=K*(1-exp(-t(i)/Tcosnt));
end
x(Tconst/0.001+1)
plot(t,x,'b',t,r,'--r');
grid on
axis([0 0.1 -0.1 1.2])
xlabel('step response of first order system')
