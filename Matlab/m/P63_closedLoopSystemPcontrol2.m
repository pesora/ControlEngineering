[t y1]=Step2nd(30);
[t y2]=Step2nd(60);
[t y3]=Step2nd(90);
[t y4]=Step2nd(120);
for i=1:1:301,
    r(i)=1;
end
plot(t,y1,'b',t,r,'--r')
hold on
plot(t,y2,'g')
plot(t,y3,'y')
plot(t,y4,'k')
grid on


 