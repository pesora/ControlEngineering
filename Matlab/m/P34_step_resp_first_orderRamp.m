Tcosnt=0.024;
t=[0:0.001:0.2];
for i=1:1:201,
r(i)=t(i);
x(i)=K*(t(i)-Tconst+Tconst*exp(-t(i)/Tcosnt));
end
ess=r(201)-x(201)
plot(t,x,'b',t,r,'--r');
grid on
%axis([0 0.2 -0.1 1.2])
xlabel('ramp response of first order system')
