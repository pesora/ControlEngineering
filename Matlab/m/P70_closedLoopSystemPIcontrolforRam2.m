clear all
t=[0:0.001:0.3];
for i=1:1:301,
    r(i)=1;
end
y1=lsim2ndPI(t,r,60,0);	 %Kp=60, Ki=0
y2=lsim2ndPI(t,r,60,100); %Kp=60, Ki=100
y3=lsim2ndPI(t,r,60,200); %Kp=60, Ki=200
hold off
plot(t,y1,'-b',t,r,'-r');	%Kp만 있는 경우(blue)
hold on
plot(t,y2,'-m')		%Ki=100인 경우(magenta)
plot(t,y3,'-k')		%Ki=200인 경우(black)
