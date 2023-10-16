Kp=60;K=1;Tconst=0.024;
numc=[Kp];
denc=[1];
nump=[K];
denp=[Tconst 1 0];
[num,den]=series(numc,denc,nump,denp);
[num,den]=cloop(num,den,-1);
printsys(num,den)
t=[0:0.001:0.3];
for i=1:1:301,
    r(i)=1;
end
y=step(num,den,t);
plot(t,y,'b',t,r,'--r')


 